/****************************************************************************
	jit.openni
	Copyright (C) 2011 Dale Phurrough

	This file is part of jit.openni.

    jit.openni is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    jit.openni is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with jit.openni.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

/**
	 jit.openni - a Max Jitter external for OpenNI middleware
	 Shell of it was imspired by the jit.simple example from the MaxSDK and
	 the MaxSDK documentation
*/


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "targetver.h"
#include "jit.openni.h"


//---------------------------------------------------------------------------
// Code
//---------------------------------------------------------------------------

// globals
static void *s_jit_openni_class = NULL;


/************************************************************************************/

t_jit_err jit_openni_init(void) 
{
	t_jit_object	*attr;
	t_jit_object	*mop;
	void			*output;
	t_atom			a_arr[3];
	int				i;

	s_jit_openni_class = jit_class_new("jit_openni", (method)jit_openni_new, (method)jit_openni_free, sizeof(t_jit_openni), 0);

	// add matrix operator (mop)
	mop = (t_jit_object*)jit_object_new(_jit_sym_jit_mop, 0, 2); // no matrix inputs, 2 matrix outputs (depth and image generator output) + default dumpout
																 // TODO allow arbitrary generator(s) therefore output(s)

	output = jit_object_method(mop,_jit_sym_getoutput,1);		// get and then set DEPTHmap plane, dimension, and type restrictions
	jit_attr_setlong(output,_jit_sym_minplanecount,1);			// TODO support whatever dimensions/planes that a generator can do based on XML config
	jit_attr_setlong(output,_jit_sym_maxplanecount,1);
	jit_attr_setlong(output,_jit_sym_mindimcount,2);
	jit_attr_setlong(output,_jit_sym_maxdimcount,2);
	jit_atom_setsym(&a_arr[0], _jit_sym_long);
	jit_atom_setsym(&a_arr[1], _jit_sym_float32);
	jit_atom_setsym(&a_arr[2], _jit_sym_float64);
	jit_object_method(output,_jit_sym_types,3,a_arr);

	jit_mop_output_nolink(mop,2);
	output = jit_object_method(mop,_jit_sym_getoutput,2);		// get and then set IMAGEmap plane, dimension, and type restrictions
	jit_attr_setlong(output,_jit_sym_minplanecount,1);
	jit_attr_setlong(output,_jit_sym_maxplanecount,4);
	jit_attr_setlong(output,_jit_sym_mindimcount,2);
	jit_attr_setlong(output,_jit_sym_maxdimcount,2);
	//jit_atom_setlong(&a_arr[0], 640);
	//jit_atom_setlong(&a_arr[1], 480);
	//jit_object_method(output,_jit_sym_maxdim,2,a_arr);
	//jit_object_method(output,_jit_sym_mindim,2,a_arr);
	jit_atom_setsym(&a_arr[0], _jit_sym_char);						// set to be the default (and only) type allowed
	jit_atom_setsym(&a_arr[1], _jit_sym_long);
	jit_object_method(output,_jit_sym_types,2,a_arr);
	
	jit_class_addadornment(s_jit_openni_class, mop);

	// add method(s)
	jit_class_addmethod(s_jit_openni_class, (method)jit_openni_matrix_calc, "matrix_calc", A_CANT, 0);
	jit_class_addmethod(s_jit_openni_class, (method)jit_openni_init_from_xml, "init_from_xml", A_SYM, 0);

	// add attribute(s)
	//	e.g. place back in decl section as needed: long attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW;



	// finalize class
	jit_class_register(s_jit_openni_class);
	return JIT_ERR_NONE;
}


/************************************************************************************/
// Object Life Cycle

t_jit_openni *jit_openni_new(void)
{
	t_jit_openni	*x = NULL;
	
	x = (t_jit_openni*)jit_object_alloc(s_jit_openni_class);
	if (x)
	{
		x->pDepthMD = xnAllocateDepthMetaData();
		x->pImageMD = xnAllocateImageMetaData();
	} 
	LOG_DEBUG("object created");
	return x;
}


void jit_openni_free(t_jit_openni *x)
{
	xnFreeDepthMetaData(x->pDepthMD);
	xnFreeImageMetaData(x->pImageMD);
	xnShutdown(x->pContext);
	LOG_DEBUG("object freed");
}


t_jit_err jit_openni_matrix_calc(t_jit_openni *x, void *inputs, void *outputs)
{
	// TODO parallelize this with a jit_openni_calculate_ndim

	t_jit_err err=JIT_ERR_NONE;
	//long in_savelock;
	long out_savelock, out2_savelock;
	//t_jit_matrix_info in_minfo;
	//t_jit_matrix_info tmp_minfo
	t_jit_matrix_info out_minfo, out2_minfo;
	//char *in_bp
	char *out_bp, *out2_bp;		// char* so can reference down to a single byte as needed
	long i, dimcount, planecount, dim[JIT_MATRIX_MAX_DIMCOUNT];
	//void *in_matrix;
	void *out_matrix, *out2_matrix, *tmp_matrix=NULL;
	XnStatus nRetVal = XN_STATUS_OK;
	//XnDepthPixel* pDepthMap;
	XnUInt32 nMiddleIndex;

	// get the zeroth index input and output from
	// the corresponding input and output lists
	//in_matrix = jit_object_method(inputs,_jit_sym_getindex,0);
	out_matrix = jit_object_method(outputs,_jit_sym_getindex,0);
	out2_matrix = jit_object_method(outputs,_jit_sym_getindex,1);
	
	// if the object and both input and output matrices, both generators are valid, then process else error
	if (x && out_matrix && out2_matrix && (x->hDepth) && (x->hImage))	// TODO allow arbitrary generator(s)
	{
		// lock input and output matrices
		//in_savelock = (long) jit_object_method(in_matrix,_jit_sym_lock,1);
		out_savelock = (long) jit_object_method(out_matrix,_jit_sym_lock,1);
		out2_savelock = (long) jit_object_method(out2_matrix,_jit_sym_lock,1);
	
		// fill out matrix info structs for input and output
		//jit_object_method(in_matrix,_jit_sym_getinfo,&in_minfo);
		jit_object_method(out_matrix,_jit_sym_getinfo,&out_minfo);
		jit_object_method(out2_matrix,_jit_sym_getinfo,&out2_minfo);

		// setup the outputs to describe the depthmap TODO don't do this every time
		out_minfo.dim[0] = x->pDepthMD->pMap->FullRes.X;
		out_minfo.dim[1] = x->pDepthMD->pMap->FullRes.Y;
		jit_object_method(out_matrix, _jit_sym_setinfo, &out_minfo);

		// setup the outputs to describe the imagemap TODO don't do this every time
		switch(x->pImageMD->pMap->PixelFormat)
		{
			case XN_PIXEL_FORMAT_RGB24:
				LOG_DEBUG("XN_PIXEL_FORMAT_RGB24");
				out2_minfo.type = _jit_sym_char;
				out2_minfo.planecount = 4;
				out2_minfo.dim[0] = x->pImageMD->pMap->FullRes.X;
				out2_minfo.dim[1] = x->pImageMD->pMap->FullRes.Y;
				break;
			case XN_PIXEL_FORMAT_YUV422:
				LOG_DEBUG("XN_PIXEL_FORMAT_YUV422");
				out2_minfo.type = _jit_sym_char;
				out2_minfo.planecount = 4;
				out2_minfo.dim[0] = x->pImageMD->pMap->FullRes.X / 2;	// trusting that X res will always be an even number
				out2_minfo.dim[1] = x->pImageMD->pMap->FullRes.Y;
				break;
			case XN_PIXEL_FORMAT_GRAYSCALE_8_BIT:
				LOG_DEBUG("XN_PIXEL_FORMAT_GRAYSCALE_8_BIT");
				out2_minfo.type = _jit_sym_char;
				out2_minfo.planecount = 1;
				out2_minfo.dim[0] = x->pImageMD->pMap->FullRes.X;
				out2_minfo.dim[1] = x->pImageMD->pMap->FullRes.Y;
				break;
			case XN_PIXEL_FORMAT_GRAYSCALE_16_BIT:
				LOG_DEBUG("XN_PIXEL_FORMAT_GRAYSCALE_16_BIT");
				out2_minfo.type = _jit_sym_long;
				out2_minfo.planecount = 1;
				out2_minfo.dim[0] = x->pImageMD->pMap->FullRes.X;
				out2_minfo.dim[1] = x->pImageMD->pMap->FullRes.Y;
				break;
			default:
				LOG_ERROR("Unsupported imagemap pixel format");
				err=JIT_ERR_MISMATCH_TYPE;
				goto out;
		}
		jit_object_method(out2_matrix, _jit_sym_setinfo, &out2_minfo);
		
		// get matrix data pointers
		//jit_object_method(in_matrix,_jit_sym_getdata,&in_bp);
		jit_object_method(out_matrix,_jit_sym_getdata,&out_bp);
		jit_object_method(out2_matrix,_jit_sym_getdata,&out2_bp);
		
		// if data pointers are invalid, set error, and cleanup
		//if (!in_bp) { err=JIT_ERR_INVALID_INPUT; goto out;}
		if (!out_bp) { err=JIT_ERR_INVALID_OUTPUT; goto out;}
		if (!out2_bp) { err=JIT_ERR_INVALID_OUTPUT; goto out;}
		
		// get dimensions/planecount
		dimcount = out_minfo.dimcount;
		planecount = out_minfo.planecount;
		for (i=0;i<dimcount;i++)
		{
			//dim[i] = out_minfo.dim[i];
			LOG_DEBUG3("out1 dimension[%d] size=%d", i, out_minfo.dim[i]);
		}
		dimcount = out2_minfo.dimcount;
		planecount = out2_minfo.planecount;
		for (i=0;i<dimcount;i++)
		{
			//dim[i] = out_minfo.dim[i];
			LOG_DEBUG3("out2 dimension[%d] size=%d", i, out2_minfo.dim[i]);
		}

		// calculate, using the parallel utility function to
		// call the calculate_ndim function in multiple
		// threads if there are multiple processors available
		//		jit_parallel_ndim_simplecalc1((method)jit_noise_calculate_ndim,	&vecdata,
		//			dimcount, dim, planecount, &out_minfo, out_bp, 0 /* flags1 */);

		// Don't wait for new data, just update all generators with the newest already available
		nRetVal = xnWaitNoneUpdateAll(x->pContext);
		if (nRetVal != XN_STATUS_OK)
		{
			LOG_ERROR2("Failed updating generator nodes", xnGetStatusString(nRetVal));
		}
		else
		{
			LOG_DEBUG("updated generator");

			// manually copy depth array to jitter matrix because depth array is 16-bit unsigned ints and jitter method don't directly support them
			copyDepthDatatoJitterMatrix(x->pDepthMD, out_bp, &out_minfo);

			// manually copy image array to jitter matrix because we may need to add alpha channel to matrix
			copyImageDatatoJitterMatrix(x->pImageMD, out2_bp, &out2_minfo);
		}
	}
	else
	{
		return JIT_ERR_INVALID_PTR;
	}
out:
	// restore matrix lock state to previous value
	jit_object_method(out_matrix,_jit_sym_lock,out_savelock);
	jit_object_method(out2_matrix,_jit_sym_lock,out2_savelock);
	//jit_object_method(in_matrix,_jit_sym_lock,in_savelock);
	return err;
}

void copy16BitDatatoJitterMatrix(XnDepthMetaData *pMapMetaData, char *bpOutJitterMatrix, t_jit_matrix_info *pOutJitterMatrixInfo)
{
	// this function assumes all parameters are valid
	
	int i, j;
	XnUInt16 *p16BitData; // aka XnDepthPixel or 16bit greysacale xnImagePixel
	
	p16BitData = (XnUInt16 *)pMapMetaData->pData;  // this ->pData assumes XnDepthMetaData struct, so XnImageMetaData must have pData in the same location TODO consider safer code
	for(i=0; i < pOutJitterMatrixInfo->dim[1]; i++)  // for each row
	{
		for(j=0; j < pOutJitterMatrixInfo->dim[0]; j++)  // go across each column
		{
			if (pOutJitterMatrixInfo->type == _jit_sym_long)
			{
				((unsigned long *)bpOutJitterMatrix)[j] = *p16BitData;
			}
			else if (pOutJitterMatrixInfo->type == _jit_sym_float32)
			{
				((float *)bpOutJitterMatrix)[j] = *p16BitData;
			}
			else // it is _jit_sym_float64
			{
				((double *)bpOutJitterMatrix)[j] = *p16BitData;
			}
			p16BitData++;
		}
		bpOutJitterMatrix += pOutJitterMatrixInfo->dimstride[1];
	}
}

void copyImageDatatoJitterMatrix(XnImageMetaData *pImageMapMetaData, char *bpOutJitterMatrix, t_jit_matrix_info *pOutJitterMatrixInfo)
{
	// this function assumes all parameters are valid
	
	int i, j;
	
	if (pImageMapMetaData->pMap->PixelFormat != XN_PIXEL_FORMAT_GRAYSCALE_16_BIT)
	{
		XnUInt8 *pImageMap = (XnUInt8 *)pImageMapMetaData->pData;
		for(i=0; i < pOutJitterMatrixInfo->dim[1]; i++) // for each row
		{
			for(j=0; j < pOutJitterMatrixInfo->dim[0]; j++)  // go across each column
			{
				switch(pImageMapMetaData->pMap->PixelFormat)
				{
					case XN_PIXEL_FORMAT_RGB24:
						((unsigned long *)bpOutJitterMatrix)[j] = MAKEULONGFROMCHARS(0xFF, pImageMap[0], pImageMap[1], pImageMap[2]); // not tested on big endian systems
						pImageMap += 3;
						break;
					case XN_PIXEL_FORMAT_YUV422:	// TODO can likely use jitter's 4-plane matrix copying function for this, ordering is U, Y1, V, Y2
						((unsigned long *)bpOutJitterMatrix)[j] = MAKEULONGFROMCHARS(pImageMap[0], pImageMap[1], pImageMap[2], pImageMap[3]); // not tested on big endian systems
						pImageMap += 4;
						break;
					case XN_PIXEL_FORMAT_GRAYSCALE_8_BIT: // TODO could likely use jitter's 1-plane matrix copying function for this, also could support long, float32, float64 output matrices
						bpOutJitterMatrix[j] = *pImageMap++;
						break;
					// case XN_PIXEL_FORMAT_GRAYSCALE_16_BIT is now handled below by calling a shared function copy16BitDatatoJitterMatrix()
				}
			}
			bpOutJitterMatrix += pOutJitterMatrixInfo->dimstride[1];
		}
	}
	else
	{
		copy16BitDatatoJitterMatrix((XnDepthMetaData *)pImageMapMetaData, bpOutJitterMatrix, pOutJitterMatrixInfo);
	}
}

void jit_openni_init_from_xml(t_jit_openni *x, t_symbol *s) // TODO should this return a t_jit_err?
{
	XnEnumerationErrors* pErrors;
	XnStatus nRetVal = XN_STATUS_OK;

	nRetVal = xnEnumerationErrorsAllocate(&pErrors);
	CHECK_RC_ERROR_EXIT(nRetVal, "jit_openni_init_from_xml: Allocate errors object");

	LOG_DEBUG2("XMLconfig loading: %s", s->s_name);
	nRetVal = xnInitFromXmlFile(s->s_name, &(x->pContext), pErrors);

	if (nRetVal == XN_STATUS_NO_NODE_PRESENT)
	{
		XnChar strError[1024];
		xnEnumerationErrorsToString(pErrors, strError, 1024);
		LOG_ERROR2("XMLconfig initialization failed", strError);
		xnEnumerationErrorsFree(pErrors);
		return;
	}
	xnEnumerationErrorsFree(pErrors);
	if (nRetVal != XN_STATUS_OK)
	{
		CHECK_RC_ERROR_EXIT(nRetVal, "XML config initialization open failed");
	}
	
	nRetVal = xnFindExistingNodeByType(x->pContext, XN_NODE_TYPE_DEPTH, &(x->hDepth));
	if (nRetVal == XN_STATUS_OK)
	{
		xnGetDepthMetaData(x->hDepth, x->pDepthMD);
		// TODO add jitter attributes to expose current modes (fps, x, y, z, etc.)
	}
#ifdef _DEBUG
	else
	{
		LOG_WARNING_RC(nRetVal, "XMLconfig no DEPTH node created/found");
	}
#endif

	nRetVal = xnFindExistingNodeByType(x->pContext, XN_NODE_TYPE_IMAGE, &(x->hImage));
	if (nRetVal == XN_STATUS_OK)
	{
		xnGetImageMetaData(x->hImage, x->pImageMD);
		// TODO add jitter attributes to expose current modes (fps, x, y, z, etc.)
	}
#ifdef _DEBUG
	else
	{
		LOG_WARNING_RC(nRetVal, "XMLconfig no IMAGE node created/found");
	}
#endif

	if (!(x->hDepth) || !(x->hImage))	// TODO allow arbitrary generator(s)
	{
		LOG_ERROR("XMLconfig must have a DEPTH and IMAGE generator to function correctly");
		return;
	}

#ifdef _DEBUG
	if (x->hDepth)
	{
		XnMapOutputMode *depthMapModes;
		XnUInt32 i, numDepthMapModes = xnGetSupportedMapOutputModesCount(x->hDepth);
		depthMapModes = (XnMapOutputMode *)malloc(sizeof(XnMapOutputMode) * numDepthMapModes);
		xnGetSupportedMapOutputModes(x->hDepth, depthMapModes, &numDepthMapModes);
		LOG_DEBUG2("== %lu Depth modes avail==", numDepthMapModes);
		for (i=0; i<numDepthMapModes; i++)
		{
			object_post((t_object*)x, "FPS=%lu X=%lu Y=%lu Z=%u", depthMapModes[i].nFPS, depthMapModes[i].nXRes, depthMapModes[i].nYRes, xnGetDeviceMaxDepth(x->hDepth));
		}
	}

	if (x->hImage)
	{
		XnMapOutputMode *imageMapModes;
		XnUInt32 i, numImageMapModes = xnGetSupportedMapOutputModesCount(x->hImage);
		imageMapModes = (XnMapOutputMode *)malloc(sizeof(XnMapOutputMode) * numImageMapModes);
		xnGetSupportedMapOutputModes(x->hImage, imageMapModes, &numImageMapModes);
		LOG_DEBUG2("== %lu Image modes avail==", numImageMapModes);
		for (i=0; i<numImageMapModes; i++)
		{
			object_post((t_object*)x, "FPS=%lu X=%lu Y=%lu", imageMapModes[i].nFPS, imageMapModes[i].nXRes, imageMapModes[i].nYRes);
		}	
	}

	object_post((t_object*)x, "==Current active modes==");
	if (x->pDepthMD) object_post((t_object*)x, "DepthMD FPS=%lu FullX=%lu FullY=%lu Z=%u", x->pDepthMD->pMap->nFPS, x->pDepthMD->pMap->FullRes.X, x->pDepthMD->pMap->FullRes.Y, x->pDepthMD->nZRes);
	if (x->pDepthMD) object_post((t_object*)x, "DepthMD OffsetX=%lu OffsetY=%lu CropX=%lu CropY=%lu", x->pDepthMD->pMap->Offset.X, x->pDepthMD->pMap->Offset.Y, x->pDepthMD->pMap->Res.X, x->pDepthMD->pMap->Res.Y);
	if (x->pImageMD) object_post((t_object*)x, "ImageMD FPS=%lu X=%lu Y=%lu", x->pImageMD->pMap->nFPS, x->pImageMD->pMap->FullRes.X, x->pImageMD->pMap->FullRes.Y);
#endif
}




