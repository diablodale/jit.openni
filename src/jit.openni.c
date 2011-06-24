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
	t_atom			a_arr[4];
	int				i;

	s_jit_openni_class = jit_class_new("jit_openni", (method)jit_openni_new, (method)jit_openni_free, sizeof(t_jit_openni), 0);

	// add matrix operator (mop)
	mop = (t_jit_object*)jit_object_new(_jit_sym_jit_mop, 0, NUM_OPENNI_GENERATORS); // no matrix inputs, matrix outputs (generator outputs) + default dumpout
																 // TODO allow arbitrary generator(s) therefore output(s)

	jit_mop_output_nolink(mop, DEPTH_GEN_INDEX + 1);				// setup the DEPTHmap
	output = jit_object_method(mop, _jit_sym_getoutput, DEPTH_GEN_INDEX + 1);
	jit_attr_setsym(output, _jit_sym_ioname, gensym("depthgen"));	// TODO unfortunately, this renames only 1 of the 4; remaining are dim, planecount, type
	jit_attr_setlong(output,_jit_sym_minplanecount,1);
	jit_attr_setlong(output,_jit_sym_maxplanecount,1);
	jit_attr_setlong(output,_jit_sym_mindimcount,2);
	jit_attr_setlong(output,_jit_sym_maxdimcount,2);
	jit_atom_setsym(&a_arr[0], _jit_sym_long);						// set to be the default
	jit_atom_setsym(&a_arr[1], _jit_sym_float32);
	jit_atom_setsym(&a_arr[2], _jit_sym_float64);
	jit_object_method(output,_jit_sym_types,3,a_arr);

	jit_mop_output_nolink(mop, IMAGE_GEN_INDEX + 1);				// setup the rgb camera IMAGEmap
	output = jit_object_method(mop, _jit_sym_getoutput, IMAGE_GEN_INDEX + 1);
	jit_attr_setsym(output, _jit_sym_ioname, gensym("imagegen"));
	jit_attr_setlong(output,_jit_sym_minplanecount,1);
	jit_attr_setlong(output,_jit_sym_maxplanecount,4);
	jit_attr_setlong(output,_jit_sym_mindimcount,2);
	jit_attr_setlong(output,_jit_sym_maxdimcount,2);
	//jit_atom_setlong(&a_arr[0], 640);
	//jit_atom_setlong(&a_arr[1], 480);
	//jit_object_method(output,_jit_sym_maxdim,2,a_arr);
	//jit_object_method(output,_jit_sym_mindim,2,a_arr);
	jit_atom_setsym(&a_arr[0], _jit_sym_char);						// set to be the default
	jit_atom_setsym(&a_arr[1], _jit_sym_long);
	jit_atom_setsym(&a_arr[2], _jit_sym_float32);
	jit_atom_setsym(&a_arr[3], _jit_sym_float64);
	jit_object_method(output,_jit_sym_types,4,a_arr);
	
	jit_mop_output_nolink(mop, USER_GEN_INDEX + 1);					// setup the user generator (skeleton)
	output = jit_object_method(mop,_jit_sym_getoutput, USER_GEN_INDEX + 1);
	jit_attr_setsym(output, _jit_sym_ioname, gensym("usergen"));
/*
	jit_attr_setlong(output,_jit_sym_minplanecount,1);
	jit_attr_setlong(output,_jit_sym_maxplanecount,4);
	jit_attr_setlong(output,_jit_sym_mindimcount,2);
	jit_attr_setlong(output,_jit_sym_maxdimcount,2);
	//jit_atom_setlong(&a_arr[0], 640);
	//jit_atom_setlong(&a_arr[1], 480);
	//jit_object_method(output,_jit_sym_maxdim,2,a_arr);
	//jit_object_method(output,_jit_sym_mindim,2,a_arr);
	jit_atom_setsym(&a_arr[0], _jit_sym_char);					// set to be the default
	jit_atom_setsym(&a_arr[1], _jit_sym_long);
	jit_object_method(output,_jit_sym_types,2,a_arr);
*/

	jit_mop_output_nolink(mop, IR_GEN_INDEX + 1);				// setup the IR camera generator
	output = jit_object_method(mop,_jit_sym_getoutput, IR_GEN_INDEX + 1);
	jit_attr_setsym(output, _jit_sym_ioname, gensym("irgen"));
	jit_attr_setlong(output,_jit_sym_minplanecount,1);
	jit_attr_setlong(output,_jit_sym_maxplanecount,1);
	jit_attr_setlong(output,_jit_sym_mindimcount,2);
	jit_attr_setlong(output,_jit_sym_maxdimcount,2);
	jit_atom_setsym(&a_arr[0], _jit_sym_long);					// set to be the default and only type allowed
	jit_object_method(output,_jit_sym_types,1,a_arr);

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
		XnStatus nRetVal = XN_STATUS_OK;

		LOG_DEBUG("Initializing OpenNI library");
		if (nRetVal = xnInit(&(x->pContext)))
		{
			LOG_ERROR("jit_openni_new: cannot initialize OpenNI");
		}
		else
		{
			x->pMapMetaData[DEPTH_GEN_INDEX] = (XnMapMetaData *)xnAllocateDepthMetaData();
			x->pMapMetaData[IMAGE_GEN_INDEX] = (XnMapMetaData *)xnAllocateImageMetaData();
			x->pMapMetaData[IR_GEN_INDEX] = (XnMapMetaData *)xnAllocateIRMetaData();
			//x->pDepthMD = xnAllocateDepthMetaData();
			//x->pImageMD = xnAllocateImageMetaData();
			//x->pIrMD = xnAllocateIRMetaData();
			//x->pSceneMD = xnAllocateSceneMetaData();
		}
	} 
	LOG_DEBUG("object created");
	return x;
}


void jit_openni_free(t_jit_openni *x)
{
	xnFreeDepthMetaData((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX]);
	xnFreeImageMetaData((XnImageMetaData *)x->pMapMetaData[IMAGE_GEN_INDEX]);
	xnFreeIRMetaData((XnIRMetaData *)x->pMapMetaData[IR_GEN_INDEX]);
	//xnFreeDepthMetaData(x->pDepthMD);
	//xnFreeImageMetaData(x->pImageMD);
	//xnFreeIRMetaData(x->pIrMD);
	//xnFreeSceneMetaData(x->pSceneMD);
	LOG_DEBUG("Shutting down OpenNI library");
	xnShutdown(x->pContext);
	LOG_DEBUG("object freed");
}


t_jit_err jit_openni_matrix_calc(t_jit_openni *x, void *inputs, void *outputs)
{
	// TODO parallelize this with a jit_openni_calculate_ndim

	t_jit_err err=JIT_ERR_NONE;
	long out_savelock[NUM_OPENNI_GENERATORS];
	t_jit_matrix_info out_minfo[NUM_OPENNI_GENERATORS];
	char *out_bp[NUM_OPENNI_GENERATORS];	// char* so can reference down to a single byte as needed
	long i, j, dimcount;
	void *out_matrix[NUM_OPENNI_GENERATORS];
	boolean bGotOutMatrices = true;
	XnStatus nRetVal = XN_STATUS_OK;
	XnUInt32 nMiddleIndex;

	// get the zeroth index input and output from
	// the corresponding input and output lists
	for (i=0;i<NUM_OPENNI_GENERATORS;i++)
	{
		if (!(out_matrix[i] = jit_object_method(outputs,_jit_sym_getindex,i)))
		{
			LOG_DEBUG2("could not get output [%d] matrix", i);
			bGotOutMatrices = false;
		}
	}

	// if the object and both input and output matrices, both generators are valid, then process else error
	if (x && bGotOutMatrices && (x->hDepth) && (x->hImage))	// TODO allow arbitrary generator(s)
	{
		// lock input and output matrices
		for (i = 0; i< NUM_OPENNI_GENERATORS; i++)
		{
			out_savelock[i] = (long) jit_object_method(out_matrix[i],_jit_sym_lock,1);
		}
	
		// fill out matrix info structs for input and output
		for (i = 0; i< NUM_OPENNI_GENERATORS; i++)
		{
			jit_object_method(out_matrix[i],_jit_sym_getinfo,&out_minfo[i]);
		}

		// BUGBUG put xnUpdate here
		nRetVal = xnWaitNoneUpdateAll(x->pContext);
		if (nRetVal != XN_STATUS_OK)
		{
			LOG_ERROR2("Failed updating generator nodes", xnGetStatusString(nRetVal));
			err = JIT_ERR_DATA_UNAVAILABLE;
		}
		else
		{
			LOG_DEBUG("updated generators");
			
			// setup the outputs to describe the depthmap TODO don't do this every time, only when it changes
			out_minfo[DEPTH_GEN_INDEX].dim[0] = ((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX])->pMap->FullRes.X;
			out_minfo[DEPTH_GEN_INDEX].dim[1] = ((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX])->pMap->FullRes.Y;
			jit_object_method(out_matrix[DEPTH_GEN_INDEX], _jit_sym_setinfo, &out_minfo[DEPTH_GEN_INDEX]);

			// setup the outputs to describe the imagemap TODO don't do this every time, only when it changes
			switch(((XnImageMetaData *)x->pMapMetaData[IMAGE_GEN_INDEX])->pMap->PixelFormat)
			{
				case XN_PIXEL_FORMAT_RGB24:
					LOG_DEBUG("XN_PIXEL_FORMAT_RGB24");
					out_minfo[IMAGE_GEN_INDEX].type = _jit_sym_char;
					out_minfo[IMAGE_GEN_INDEX].planecount = 4;
					out_minfo[IMAGE_GEN_INDEX].dim[0] = ((XnImageMetaData *)x->pMapMetaData[IMAGE_GEN_INDEX])->pMap->FullRes.X;
					out_minfo[IMAGE_GEN_INDEX].dim[1] = ((XnImageMetaData *)x->pMapMetaData[IMAGE_GEN_INDEX])->pMap->FullRes.Y;
					break;
				case XN_PIXEL_FORMAT_YUV422:
					LOG_DEBUG("XN_PIXEL_FORMAT_YUV422");
					out_minfo[IMAGE_GEN_INDEX].type = _jit_sym_char;
					out_minfo[IMAGE_GEN_INDEX].planecount = 4;
					out_minfo[IMAGE_GEN_INDEX].dim[0] = ((XnImageMetaData *)x->pMapMetaData[IMAGE_GEN_INDEX])->pMap->FullRes.X / 2;	// trusting that X res will always be an even number
					out_minfo[IMAGE_GEN_INDEX].dim[1] = ((XnImageMetaData *)x->pMapMetaData[IMAGE_GEN_INDEX])->pMap->FullRes.Y;
					break;
				case XN_PIXEL_FORMAT_GRAYSCALE_8_BIT:
					LOG_DEBUG("XN_PIXEL_FORMAT_GRAYSCALE_8_BIT");
					out_minfo[IMAGE_GEN_INDEX].type = _jit_sym_char;
					out_minfo[IMAGE_GEN_INDEX].planecount = 1;
					out_minfo[IMAGE_GEN_INDEX].dim[0] = ((XnImageMetaData *)x->pMapMetaData[IMAGE_GEN_INDEX])->pMap->FullRes.X;
					out_minfo[IMAGE_GEN_INDEX].dim[1] = ((XnImageMetaData *)x->pMapMetaData[IMAGE_GEN_INDEX])->pMap->FullRes.Y;
					break;
				case XN_PIXEL_FORMAT_GRAYSCALE_16_BIT:
					LOG_DEBUG("XN_PIXEL_FORMAT_GRAYSCALE_16_BIT");
					out_minfo[IMAGE_GEN_INDEX].type = _jit_sym_long;
					out_minfo[IMAGE_GEN_INDEX].planecount = 1;
					out_minfo[IMAGE_GEN_INDEX].dim[0] = ((XnImageMetaData *)x->pMapMetaData[IMAGE_GEN_INDEX])->pMap->FullRes.X;
					out_minfo[IMAGE_GEN_INDEX].dim[1] = ((XnImageMetaData *)x->pMapMetaData[IMAGE_GEN_INDEX])->pMap->FullRes.Y;
					break;
				default:
					LOG_ERROR("Unsupported imagemap pixel format");
					err=JIT_ERR_MISMATCH_TYPE;
					goto out;
			}
			jit_object_method(out_matrix[IMAGE_GEN_INDEX], _jit_sym_setinfo, &out_minfo[IMAGE_GEN_INDEX]);
		
			// get matrix data pointers
			for (i = 0; i< NUM_OPENNI_GENERATORS; i++)
			{
				jit_object_method(out_matrix[i],_jit_sym_getdata,&out_bp[i]);
				if (!out_bp[i]) { err=JIT_ERR_INVALID_OUTPUT; goto out;} // if data pointers are invalid, set error, and cleanup

#ifdef _DEBUG
				// get dimensions/planecount
				dimcount = out_minfo[i].dimcount;
				for (j=0;j<dimcount;j++)
				{
					object_post((t_object*)x, "out%d dim[%d] = %d", i, j, out_minfo[i].dim[j]);
				}
				LOG_DEBUG3("out%d planes = %d", i, out_minfo[i].planecount);
#endif
			}
		
			// calculate, using the parallel utility function to
			// call the calculate_ndim function in multiple
			// threads if there are multiple processors available
			//		jit_parallel_ndim_simplecalc1((method)jit_noise_calculate_ndim,	&vecdata,
			//			dimcount, dim, planecount, &out_minfo, out_bp, 0 /* flags1 */);

			// Don't wait for new data, just update all generators with the newest already available
			// TODO adjust codebase to optionally allow no new matrices to be output for generators that have no new data
			
			//nRetVal = xnWaitNoneUpdateAll(x->pContext);
			//if (nRetVal != XN_STATUS_OK)
			//{
			//	LOG_ERROR2("Failed updating generator nodes", xnGetStatusString(nRetVal));
			//}
			//else
			//{
			//LOG_DEBUG("updated generator(s)");

			// manually copy depth array to jitter matrix because depth array is 16-bit unsigned ints and jitter method don't directly support them
			copyDepthDatatoJitterMatrix(((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX]), out_bp[DEPTH_GEN_INDEX], &out_minfo[DEPTH_GEN_INDEX]);

			// manually copy image array to jitter matrix because we may need to add alpha channel to matrix
			copyImageDatatoJitterMatrix(((XnImageMetaData *)x->pMapMetaData[IMAGE_GEN_INDEX]), out_bp[IMAGE_GEN_INDEX], &out_minfo[IMAGE_GEN_INDEX]);
		}
	}
	else
	{
		return JIT_ERR_INVALID_PTR;
	}
out:
	// restore matrix lock state to previous value
	for (i = 0; i< NUM_OPENNI_GENERATORS; i++)
	{
		jit_object_method(out_matrix[i],_jit_sym_lock,out_savelock[i]);
	}
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
	CHECK_RC_ERROR_EXIT(nRetVal, "jit_openni_init_from_xml: cannot allocate errors object");

	nRetVal = xnStopGeneratingAll(x->pContext);		// should stop generators in case we are loading a new XML file
	CHECK_RC_ERROR_EXIT(nRetVal, "jit_openni_init_from_xml: cannot stop all generators before loading XML config");

	LOG_DEBUG2("XMLconfig loading: %s", s->s_name);
	nRetVal = xnContextRunXmlScriptFromFile(x->pContext, s->s_name, pErrors);	// TODO this doesn't seem to support loading a 2nd XML file

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
		xnGetDepthMetaData(x->hDepth, (XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX]);
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
		xnGetImageMetaData(x->hImage, (XnImageMetaData *)x->pMapMetaData[IMAGE_GEN_INDEX]);
	}
#ifdef _DEBUG
	else
	{
		LOG_WARNING_RC(nRetVal, "XMLconfig no IMAGE node created/found");
	}
#endif

	nRetVal = xnFindExistingNodeByType(x->pContext, XN_NODE_TYPE_USER, &(x->hUser));
	if (nRetVal == XN_STATUS_OK)
	{
		LOG_DEBUG("may want to do some kind of USER generator preload here");
	}
#ifdef _DEBUG
	else
	{
		LOG_WARNING_RC(nRetVal, "XMLconfig no USER node created/found");
	}
#endif

	nRetVal = xnFindExistingNodeByType(x->pContext, XN_NODE_TYPE_IR, &(x->hIr));
	if (nRetVal == XN_STATUS_OK)
	{
		xnGetIRMetaData(x->hIr, (XnIRMetaData *)x->pMapMetaData[IR_GEN_INDEX]);
	}
#ifdef _DEBUG
	else
	{
		LOG_WARNING_RC(nRetVal, "XMLconfig no IR node created/found");
	}
#endif


	if (!(x->hDepth || x->hImage || x->hUser || x->hIr))
	{
		LOG_ERROR("XMLconfig must have at least one generator (other than recorder) to function correctly");
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

//	if (x->hUser)
//	{
//	}
	
	if (x->hIr)
	{
		XnMapOutputMode *IrMapModes;
		XnUInt32 i, numIrMapModes = xnGetSupportedMapOutputModesCount(x->hIr);
		IrMapModes = (XnMapOutputMode *)malloc(sizeof(XnMapOutputMode) * numIrMapModes);
		xnGetSupportedMapOutputModes(x->hIr, IrMapModes, &numIrMapModes);
		LOG_DEBUG2("== %lu IR modes avail==", numIrMapModes);
		for (i=0; i<numIrMapModes; i++)
		{
			object_post((t_object*)x, "FPS=%lu X=%lu Y=%lu", IrMapModes[i].nFPS, IrMapModes[i].nXRes, IrMapModes[i].nYRes);
		}	
	}

	object_post((t_object*)x, "==Current active modes==");
	if (x->hDepth)
	{
		object_post((t_object*)x, "DepthMD FPS=%lu FullX=%lu FullY=%lu Z=%u", ((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX])->pMap->nFPS, ((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX])->pMap->FullRes.X, ((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX])->pMap->FullRes.Y, ((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX])->nZRes);
		object_post((t_object*)x, "DepthMD OffsetX=%lu OffsetY=%lu CropX=%lu CropY=%lu", ((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX])->pMap->Offset.X, ((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX])->pMap->Offset.Y, ((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX])->pMap->Res.X, ((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX])->pMap->Res.Y);
		object_post((t_object*)x, "DepthMD PixelFormat=%s", xnPixelFormatToString(((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX])->pMap->PixelFormat));
	}
	if (x->hImage)
	{
		object_post((t_object*)x, "ImageMD FPS=%lu X=%lu Y=%lu", ((XnImageMetaData *)x->pMapMetaData[IMAGE_GEN_INDEX])->pMap->nFPS, ((XnImageMetaData *)x->pMapMetaData[IMAGE_GEN_INDEX])->pMap->FullRes.X, ((XnImageMetaData *)x->pMapMetaData[IMAGE_GEN_INDEX])->pMap->FullRes.Y);
		object_post((t_object*)x, "ImageMD PixelFormat=%s", xnPixelFormatToString(((XnImageMetaData *)x->pMapMetaData[IMAGE_GEN_INDEX])->pMap->PixelFormat));
	}
	// TODO something for USER generator	
	if (x->hIr)
	{
		object_post((t_object*)x, "IrMD FPS=%lu X=%lu Y=%lu", ((XnIRMetaData *)x->pMapMetaData[IR_GEN_INDEX])->pMap->nFPS, ((XnIRMetaData *)x->pMapMetaData[IR_GEN_INDEX])->pMap->FullRes.X, ((XnIRMetaData *)x->pMapMetaData[IR_GEN_INDEX])->pMap->FullRes.Y);
		object_post((t_object*)x, "IrMD PixelFormat=%s", xnPixelFormatToString(((XnIRMetaData *)x->pMapMetaData[IR_GEN_INDEX])->pMap->PixelFormat));
	}
#endif
}




