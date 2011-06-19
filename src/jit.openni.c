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
	
	s_jit_openni_class = jit_class_new("jit_openni", (method)jit_openni_new, (method)jit_openni_free, sizeof(t_jit_openni), 0);

	// add matrix operator (mop)
	mop = (t_jit_object*)jit_object_new(_jit_sym_jit_mop, 0, 2); // no matrix inputs, 2 matrix outputs (depth and image generator output) + default dumpout
																 // TODO allow arbitrary generator(s) therefore output(s)
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
#ifdef _DEBUG
	LOG_COMMENT("object created");
#endif
	
	return x;
}


void jit_openni_free(t_jit_openni *x)
{
	xnFreeDepthMetaData(x->pDepthMD);
	xnFreeImageMetaData(x->pImageMD);
	xnShutdown(x->pContext);
#ifdef _DEBUG
	LOG_COMMENT("object freed");
#endif
}


t_jit_err jit_openni_matrix_calc(t_jit_openni *x, void *inputs, void *outputs)
{
	t_jit_err err=JIT_ERR_NONE;
	long in_savelock, out_savelock;
	t_jit_matrix_info in_minfo, out_minfo;
	char *in_bp,*out_bp;
	long i, dimcount, planecount, dim[JIT_MATRIX_MAX_DIMCOUNT];
	void *in_matrix, *out_matrix;
	XnStatus nRetVal = XN_STATUS_OK;
	
	// get the zeroth index input and output from
	// the corresponding input and output lists
	//in_matrix = jit_object_method(inputs,_jit_sym_getindex,0);
	out_matrix = jit_object_method(outputs,_jit_sym_getindex,0);
	
	// if the object and both input and output matrices
	// are valid, then process, else return an error
	//if (x && in_matrix && out_matrix)
	if (x && out_matrix)
	{
		// lock input and output matrices
		//in_savelock = (long) jit_object_method(in_matrix,_jit_sym_lock,1);
		out_savelock = (long) jit_object_method(out_matrix,_jit_sym_lock,1);
	
		// fill out matrix info structs for input and output
		//jit_object_method(in_matrix,_jit_sym_getinfo,&in_minfo);
		jit_object_method(out_matrix,_jit_sym_getinfo,&out_minfo);
		
		// get matrix data pointers
		//jit_object_method(in_matrix,_jit_sym_getdata,&in_bp);
		jit_object_method(out_matrix,_jit_sym_getdata,&out_bp);
		
		// if data pointers are invalid, set error, and cleanup
		//if (!in_bp) { err=JIT_ERR_INVALID_INPUT; goto out;}
		if (!out_bp) { err=JIT_ERR_INVALID_OUTPUT; goto out;}
		
		// enforce compatible types
		//if ((in_minfo.type!=_jit_sym_char) ||
		//	(in_minfo.type!=out_minfo.type))
		//{
		//	err=JIT_ERR_MISMATCH_TYPE;
		//	goto out;
		//}
		
		// enforce compatible planecount
		//if ((in_minfo.planecount!=4) ||
		//	(out_minfo.planecount!=4))
		//{
		//	err=JIT_ERR_MISMATCH_PLANE;
		//	goto out;
		//}
		
		// get dimensions/planecount
//		dimcount = out_minfo.dimcount;
//		planecount = out_minfo.planecount;
		
//		for (i=0;i<dimcount;i++)
//		{
			// if input and output are not matched in
			// size, use the intersection of the two
			//dim[i] = MIN(in_minfo.dim[i],out_minfo.dim[i]);
//			dim[i] = out_minfo.dim[i];
//		}
		
		// calculate, using the parallel utility function to
		// call the calculate_ndim function in multiple
		// threads if there are multiple processors available
//		jit_parallel_ndim_simplecalc1((method)jit_noise_calculate_ndim,	&vecdata,
//			dimcount, dim, planecount, &out_minfo, out_bp, 0 /* flags1 */);

		if ((x->hDepth) && (x->hImage))	// TODO allow arbitrary generator(s)
		{
			// Don't wait for new data, just update all generators with the newest already available
			nRetVal = xnWaitNoneUpdateAll(x->pContext);
			if (nRetVal != XN_STATUS_OK)
			{
				LOG_ERROR2("Failed updating generator nodes", xnGetStatusString(nRetVal));
			}
			else
			{
				LOG_DEBUG("updated generator");
			}
		}
	}
	else
	{
		return JIT_ERR_INVALID_PTR;
	}
out:
	// restore matrix lock state to previous value
	jit_object_method(out_matrix,_jit_sym_lock,out_savelock);
	//jit_object_method(in_matrix,_jit_sym_lock,in_savelock);
	return err;
}

void jit_openni_init_from_xml(t_jit_openni *x, t_symbol *s) // TODO should this return a t_jit_err?
{
	XnEnumerationErrors* pErrors;
	XnStatus nRetVal = XN_STATUS_OK;

	nRetVal = xnEnumerationErrorsAllocate(&pErrors);
	CHECK_RC_ERROR_EXIT(nRetVal, "jit_openni_init_from_xml: Allocate errors object");

	object_post((t_object *)x, "XMLconfig loading: %s", s->s_name);
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
		LOG_COMMENT2("== %lu Depth modes avail==", numDepthMapModes);
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
		LOG_COMMENT2("== %lu Image modes avail==", numImageMapModes);
		for (i=0; i<numImageMapModes; i++)
		{
			object_post((t_object*)x, "FPS=%lu X=%lu Y=%lu", imageMapModes[i].nFPS, imageMapModes[i].nXRes, imageMapModes[i].nYRes);
		}	
	}

	object_post((t_object*)x, "==Current active modes==");
	if (x->pDepthMD) object_post((t_object*)x, "DepthMD FPS=%lu X=%lu Y=%lu Z=%u", x->pDepthMD->pMap->nFPS, x->pDepthMD->pMap->FullRes.X, x->pDepthMD->pMap->FullRes.Y, x->pDepthMD->nZRes);
	if (x->pImageMD) object_post((t_object*)x, "ImageMD FPS=%lu X=%lu Y=%lu", x->pImageMD->pMap->nFPS, x->pImageMD->pMap->FullRes.X, x->pImageMD->pMap->FullRes.Y);
#endif
}




