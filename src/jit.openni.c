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

#ifdef DEPTH_GEN_INDEX
	//jit_mop_output_nolink(mop, DEPTH_GEN_INDEX + 1);				// if I nolink() then I can't change the attributes in the inspector
	output = jit_object_method(mop, _jit_sym_getoutput, DEPTH_GEN_INDEX + 1);
	jit_attr_setlong(output,_jit_sym_minplanecount,1);
	jit_attr_setlong(output,_jit_sym_maxplanecount,1);
	jit_attr_setlong(output,_jit_sym_mindimcount,2);
	jit_attr_setlong(output,_jit_sym_maxdimcount,2);
	jit_atom_setsym(&a_arr[0], _jit_sym_long);						// set to be the default
	jit_atom_setsym(&a_arr[1], _jit_sym_float32);
	jit_atom_setsym(&a_arr[2], _jit_sym_float64);
	jit_object_method(output,_jit_sym_types,3,a_arr);
#endif

#ifdef IMAGE_GEN_INDEX
	//jit_mop_output_nolink(mop, IMAGE_GEN_INDEX + 1);				// if I nolink() then I can't change the attributes in the inspector
	output = jit_object_method(mop, _jit_sym_getoutput, IMAGE_GEN_INDEX + 1);
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
#endif

#ifdef USER_GEN_INDEX
	//jit_mop_output_nolink(mop, USER_GEN_INDEX + 1);				// if I nolink() then I can't change the attributes in the inspector
	output = jit_object_method(mop,_jit_sym_getoutput, USER_GEN_INDEX + 1);
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
#endif

#ifdef IR_GEN_INDEX
	//jit_mop_output_nolink(mop, IR_GEN_INDEX + 1);				// if I nolink() then I can't change the attributes in the inspector
	output = jit_object_method(mop,_jit_sym_getoutput, IR_GEN_INDEX + 1);
	jit_attr_setlong(output,_jit_sym_minplanecount,1);
	jit_attr_setlong(output,_jit_sym_maxplanecount,1);
	jit_attr_setlong(output,_jit_sym_mindimcount,2);
	jit_attr_setlong(output,_jit_sym_maxdimcount,2);
	jit_atom_setsym(&a_arr[0], _jit_sym_long);					// set to be the default and only type allowed
	jit_object_method(output,_jit_sym_types,1,a_arr);
#endif

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
		x->bHaveValidGeneratorProductionNode = false;

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
	if (x->pProductionNodeList) xnNodeInfoListFree(x->pProductionNodeList);
	x->bHaveValidGeneratorProductionNode = false;
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
	if (x && bGotOutMatrices && x->bHaveValidGeneratorProductionNode)
	{
		// lock input and output matrices
		for (i = 0; i< NUM_OPENNI_GENERATORS; i++)
		{
			out_savelock[i] = (long) jit_object_method(out_matrix[i],_jit_sym_lock,1);
		}

		// Don't wait for new data, just update all generators with the newest already available
		// TODO adjust codebase to optionally allow no new matrices to be output for generators that have no new data
		nRetVal = xnWaitNoneUpdateAll(x->pContext);
		if (nRetVal != XN_STATUS_OK)
		{
			LOG_ERROR2("Failed updating generator nodes", xnGetStatusString(nRetVal));
			err = JIT_ERR_DATA_UNAVAILABLE;
		}
		else
		{
			LOG_DEBUG("updated generators");

			// BUGBUG if I change the matrix type using the inspector while it is processing matrices, Max crashes
			// TODO don't do this every time, only when generators change
			for (i = 0; i< NUM_OPENNI_GENERATORS; i++)
			{
				// fill out matrix info structs for input and output
				jit_object_method(out_matrix[i],_jit_sym_getinfo,&out_minfo[i]);
				
				// BUGBUG this should instead look at productionNode and for the ones that are children of MapGenerators, do the following matrix resizing
				if (x->hProductionNode[i])	// TODO restrict this to only work for maps
				{
					LOG_DEBUG3("generator[%d] type=%s", i, xnProductionNodeTypeToString(xnNodeInfoGetDescription(xnGetNodeInfo(x->hProductionNode[i]))->Type));
					LOG_DEBUG3("generator[%d] pixelformat=%s", i, xnPixelFormatToString(((XnDepthMetaData *)x->pMapMetaData[i])->pMap->PixelFormat));
					
					// setup the outputs to support the map's PixelFormat
					switch(((XnDepthMetaData *)x->pMapMetaData[i])->pMap->PixelFormat)
					{
						case XN_PIXEL_FORMAT_RGB24:
							out_minfo[i].type = _jit_sym_char;
							out_minfo[i].planecount = 4;
							out_minfo[i].dim[0] = ((XnDepthMetaData *)x->pMapMetaData[i])->pMap->FullRes.X;
							out_minfo[i].dim[1] = ((XnDepthMetaData *)x->pMapMetaData[i])->pMap->FullRes.Y;
							break;
						case XN_PIXEL_FORMAT_YUV422:
							out_minfo[i].type = _jit_sym_char;
							out_minfo[i].planecount = 4;
							out_minfo[i].dim[0] = ((XnDepthMetaData *)x->pMapMetaData[i])->pMap->FullRes.X / 2;	// trusting that X res will always be an even number
							out_minfo[i].dim[1] = ((XnDepthMetaData *)x->pMapMetaData[i])->pMap->FullRes.Y;
							break;
						case XN_PIXEL_FORMAT_GRAYSCALE_8_BIT:
							out_minfo[i].type = _jit_sym_char;
							out_minfo[i].planecount = 1;
							out_minfo[i].dim[0] = ((XnDepthMetaData *)x->pMapMetaData[i])->pMap->FullRes.X;
							out_minfo[i].dim[1] = ((XnDepthMetaData *)x->pMapMetaData[i])->pMap->FullRes.Y;
							break;
						case XN_PIXEL_FORMAT_GRAYSCALE_16_BIT:
							out_minfo[i].planecount = 1;
							out_minfo[i].dim[0] = ((XnDepthMetaData *)x->pMapMetaData[i])->pMap->FullRes.X;
							out_minfo[i].dim[1] = ((XnDepthMetaData *)x->pMapMetaData[i])->pMap->FullRes.Y;
							break;
						default:
							LOG_ERROR2("Unsupported PixelFormat", xnPixelFormatToString(((XnDepthMetaData *)x->pMapMetaData[i])->pMap->PixelFormat));
							err=JIT_ERR_MISMATCH_TYPE;
							goto out;
					}
					jit_object_method(out_matrix[i], _jit_sym_setinfo, &out_minfo[i]);
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
				// get matrix data pointers
				jit_object_method(out_matrix[i],_jit_sym_getdata,&out_bp[i]);
				if (!out_bp[i]) { err=JIT_ERR_INVALID_OUTPUT; goto out;} // if data pointers are invalid, set error, and cleanup

				// calculate, using the parallel utility function to
				// call the calculate_ndim function in multiple
				// threads if there are multiple processors available
				//		jit_parallel_ndim_simplecalc1((method)jit_noise_calculate_ndim,	&vecdata,
				//			dimcount, dim, planecount, &out_minfo, out_bp, 0 /* flags1 */);

				// manually copy OpenNI arrays to jitter matrix because jitter doesn't directly support them
				copyMapGenDatatoJitterMatrix((XnImageMetaData *)x->pMapMetaData[i], out_bp[i], &out_minfo[i]);
			}
		}
	}
	else
	{
		return JIT_ERR_DATA_UNAVAILABLE;
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
	// this function assumes all parameters are valid, and requires that all map metadata's passed via pImageMapMetaData have the same byte location of the 	
	
	int i, j;
	XnUInt16 *p16BitData; // aka XnDepthPixel or 16bit greysacale xnImagePixel
	
	p16BitData = (XnUInt16 *)pMapMetaData->pData;  // this ->pData assumes XnDepthMetaData struct
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

void copyMapGenDatatoJitterMatrix(XnImageMetaData *pImageMapMetaData, char *bpOutJitterMatrix, t_jit_matrix_info *pOutJitterMatrixInfo)
{
	// this function assumes all parameters are valid, and requires that all map metadata's passed via pImageMapMetaData have the same byte location of the 	
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
	XnNodeInfoListIterator pCurrentNode;
	XnNodeInfo* pProdNodeInfo;
	int i;

	nRetVal = xnEnumerationErrorsAllocate(&pErrors);
	CHECK_RC_ERROR_EXIT(nRetVal, "jit_openni_init_from_xml: cannot allocate errors object");

	nRetVal = xnStopGeneratingAll(x->pContext);		// should stop generators in case we are loading a new XML file
	CHECK_RC_ERROR_EXIT(nRetVal, "jit_openni_init_from_xml: cannot stop all generators before loading XML config");

	nRetVal = xnContextRunXmlScriptFromFile(x->pContext, s->s_name, pErrors);	// TODO this doesn't seem to support loading a 2nd XML file
																				// may need to iterate xnProductionNodeRelease() or xnShutdown()
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
	LOG_DEBUG2("XMLconfig loaded: %s", s->s_name);

	nRetVal = xnEnumerateExistingNodes(x->pContext,&x->pProductionNodeList); //TODO try this new style
	CHECK_RC_ERROR_EXIT(nRetVal, "XMLconfig cannot enumerate existing production nodes");
	for (pCurrentNode = xnNodeInfoListGetFirst(x->pProductionNodeList); xnNodeInfoListIteratorIsValid(pCurrentNode); pCurrentNode = xnNodeInfoListGetNext(pCurrentNode))
	{
		pProdNodeInfo = xnNodeInfoListGetCurrent(pCurrentNode);
		LOG_DEBUG2("found prodnode type=%s", xnProductionNodeTypeToString(xnNodeInfoGetDescription(pProdNodeInfo)->Type));
		x->bHaveValidGeneratorProductionNode = x->bHaveValidGeneratorProductionNode || xnIsTypeGenerator(xnNodeInfoGetDescription(pProdNodeInfo)->Type);
		switch(xnNodeInfoGetDescription(pProdNodeInfo)->Type)
		{
		case XN_NODE_TYPE_DEPTH:
			x->hProductionNode[DEPTH_GEN_INDEX] = xnNodeInfoGetHandle(pProdNodeInfo);
			xnGetDepthMetaData(x->hProductionNode[DEPTH_GEN_INDEX], (XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX]);
			break;
		case XN_NODE_TYPE_IMAGE:
			x->hProductionNode[IMAGE_GEN_INDEX] = xnNodeInfoGetHandle(pProdNodeInfo);
			xnGetImageMetaData(x->hProductionNode[IMAGE_GEN_INDEX], (XnImageMetaData *)x->pMapMetaData[IMAGE_GEN_INDEX]);
			break;
		case XN_NODE_TYPE_IR:
			x->hProductionNode[IR_GEN_INDEX] = xnNodeInfoGetHandle(pProdNodeInfo);
			xnGetIRMetaData(x->hProductionNode[IR_GEN_INDEX], (XnIRMetaData *)x->pMapMetaData[IR_GEN_INDEX]);
		}
	}
	LOG_DEBUG2("bHaveValidGeneratorProductionNode=%d", x->bHaveValidGeneratorProductionNode);

#ifdef _DEBUG
	if (x->hProductionNode[DEPTH_GEN_INDEX])
	{
		XnMapOutputMode *depthMapModes;
		XnUInt32 i, numDepthMapModes = xnGetSupportedMapOutputModesCount(x->hProductionNode[DEPTH_GEN_INDEX]);
		depthMapModes = (XnMapOutputMode *)malloc(sizeof(XnMapOutputMode) * numDepthMapModes);
		xnGetSupportedMapOutputModes(x->hProductionNode[DEPTH_GEN_INDEX], depthMapModes, &numDepthMapModes);
		LOG_DEBUG2("== %lu Depth modes avail==", numDepthMapModes);
		for (i=0; i<numDepthMapModes; i++)
		{
			object_post((t_object*)x, "FPS=%lu X=%lu Y=%lu Z=%u", depthMapModes[i].nFPS, depthMapModes[i].nXRes, depthMapModes[i].nYRes, xnGetDeviceMaxDepth(x->hProductionNode[DEPTH_GEN_INDEX]));
		}
	}

	if (x->hProductionNode[IMAGE_GEN_INDEX])
	{
		XnMapOutputMode *imageMapModes;
		XnUInt32 i, numImageMapModes = xnGetSupportedMapOutputModesCount(x->hProductionNode[IMAGE_GEN_INDEX]);
		imageMapModes = (XnMapOutputMode *)malloc(sizeof(XnMapOutputMode) * numImageMapModes);
		xnGetSupportedMapOutputModes(x->hProductionNode[IMAGE_GEN_INDEX], imageMapModes, &numImageMapModes);
		LOG_DEBUG2("== %lu Image modes avail==", numImageMapModes);
		for (i=0; i<numImageMapModes; i++)
		{
			object_post((t_object*)x, "FPS=%lu X=%lu Y=%lu", imageMapModes[i].nFPS, imageMapModes[i].nXRes, imageMapModes[i].nYRes);
		}	
	}

//	if (x->hProductionNode[USER_GEN_INDEX])
//	{
//	}
	
	if (x->hProductionNode[IR_GEN_INDEX])
	{
		XnMapOutputMode *IrMapModes;
		XnUInt32 i, numIrMapModes = xnGetSupportedMapOutputModesCount(x->hProductionNode[IR_GEN_INDEX]);
		IrMapModes = (XnMapOutputMode *)malloc(sizeof(XnMapOutputMode) * numIrMapModes);
		xnGetSupportedMapOutputModes(x->hProductionNode[IR_GEN_INDEX], IrMapModes, &numIrMapModes);
		LOG_DEBUG2("== %lu IR modes avail==", numIrMapModes);
		for (i=0; i<numIrMapModes; i++)
		{
			object_post((t_object*)x, "FPS=%lu X=%lu Y=%lu", IrMapModes[i].nFPS, IrMapModes[i].nXRes, IrMapModes[i].nYRes);
		}	
	}

	object_post((t_object*)x, "==Current active modes==");
	if (x->hProductionNode[DEPTH_GEN_INDEX])
	{
		object_post((t_object*)x, "DepthMD FPS=%lu FullX=%lu FullY=%lu Z=%u", ((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX])->pMap->nFPS, ((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX])->pMap->FullRes.X, ((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX])->pMap->FullRes.Y, ((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX])->nZRes);
		object_post((t_object*)x, "DepthMD OffsetX=%lu OffsetY=%lu CropX=%lu CropY=%lu", ((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX])->pMap->Offset.X, ((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX])->pMap->Offset.Y, ((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX])->pMap->Res.X, ((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX])->pMap->Res.Y);
		object_post((t_object*)x, "DepthMD PixelFormat=%s", xnPixelFormatToString(((XnDepthMetaData *)x->pMapMetaData[DEPTH_GEN_INDEX])->pMap->PixelFormat));
	}
	if (x->hProductionNode[IMAGE_GEN_INDEX])
	{
		object_post((t_object*)x, "ImageMD FPS=%lu X=%lu Y=%lu", ((XnImageMetaData *)x->pMapMetaData[IMAGE_GEN_INDEX])->pMap->nFPS, ((XnImageMetaData *)x->pMapMetaData[IMAGE_GEN_INDEX])->pMap->FullRes.X, ((XnImageMetaData *)x->pMapMetaData[IMAGE_GEN_INDEX])->pMap->FullRes.Y);
		object_post((t_object*)x, "ImageMD PixelFormat=%s", xnPixelFormatToString(((XnImageMetaData *)x->pMapMetaData[IMAGE_GEN_INDEX])->pMap->PixelFormat));
	}
	// TODO something for USER generator	
	if (x->hProductionNode[IR_GEN_INDEX])
	{
		object_post((t_object*)x, "IrMD FPS=%lu X=%lu Y=%lu", ((XnIRMetaData *)x->pMapMetaData[IR_GEN_INDEX])->pMap->nFPS, ((XnIRMetaData *)x->pMapMetaData[IR_GEN_INDEX])->pMap->FullRes.X, ((XnIRMetaData *)x->pMapMetaData[IR_GEN_INDEX])->pMap->FullRes.Y);
		object_post((t_object*)x, "IrMD PixelFormat=%s", xnPixelFormatToString(((XnIRMetaData *)x->pMapMetaData[IR_GEN_INDEX])->pMap->PixelFormat));
	}
#endif
}




