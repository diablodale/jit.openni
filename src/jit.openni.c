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
#include "jit.common.h"
#include "XnOpenNI.h"


//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------

#define LOG_ERROR(what)														\
	{																		\
		object_error((t_object*)x, what);									\
	}

#define LOG_ERROR2(what, errorstring)										\
	{																		\
		object_error((t_object*)x, "%s (%s)", what, errorstring);			\
	}

#define LOG_WARNING(what, warnstring)										\
	{																		\
		object_warn((t_object*)x, "%s (%s)", what, warnstring);				\
	}

#define LOG_COMMENT(what)													\
	{																		\
		object_post((t_object*)x, what);									\
	}

#define LOG_COMMENT2(what, commentstring)									\
	{																		\
		object_post((t_object*)x, what, commentstring);						\
	}

#define CHECK_RC_WARN(rc, what)												\
	if (rc != XN_STATUS_OK)													\
	{																		\
		LOG_WARNING(what, xnGetStatusString(rc));							\
	}

#define CHECK_RC_ERROR_EXIT(rc, what)										\
	if (rc != XN_STATUS_OK)													\
	{																		\
		object_error((t_object*)x, "%s (%s)", what, xnGetStatusString(rc));	\
		return;																\
	}



//---------------------------------------------------------------------------
// Code
//---------------------------------------------------------------------------


// Our Jitter object instance data
typedef struct _jit_openni {
	t_object	ob;
	XnContext* pContext;
	XnNodeHandle hDepth, hImage;
	XnDepthMetaData* pDepthMD;
	XnImageMetaData* pImageMD;
} t_jit_openni;


// prototypes
//BEGIN_USING_C_LINKAGE
t_jit_err		jit_openni_init				(void); 
t_jit_openni	*jit_openni_new				(void);
void			jit_openni_free				(t_jit_openni *x);
void			jit_openni_init_from_xml	(t_jit_openni *x, t_symbol *s); // TODO should this return a t_jit_err?
//END_USING_C_LINKAGE


// globals
static void *s_jit_openni_class = NULL;


/************************************************************************************/

t_jit_err jit_openni_init(void) 
{
//	long			attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW;
	t_jit_object	*attr;
	t_jit_object	*mop;
	
	s_jit_openni_class = jit_class_new("jit_openni", (method)jit_openni_new, (method)jit_openni_free, sizeof(t_jit_openni), 0);

	// add matrix operator (mop)
	mop = (t_jit_object*)jit_object_new(_jit_sym_jit_mop, 0, 2); // args are num inputs and num outputs
	jit_class_addadornment(s_jit_openni_class, mop);

	// add method(s)
	//jit_class_addmethod(s_jit_openni_class, (method)jit_openni_matrix_calc, "matrix_calc", A_CANT, 0);
	jit_class_addmethod(s_jit_openni_class, (method)jit_openni_init_from_xml, "init_from_xml", A_SYM, 0);

	// add attribute(s)

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


void jit_openni_init_from_xml(t_jit_openni *x, t_symbol *s) // TODO should this return a t_jit_err?
{
	XnEnumerationErrors* pErrors;
	XnStatus nRetVal = XN_STATUS_OK;

	nRetVal = xnEnumerationErrorsAllocate(&pErrors);
	CHECK_RC_ERROR_EXIT(nRetVal, "jit_openni_init_from_xml: Allocate errors object");

	object_post((t_object *)x, "XMLConfig to load: %s", s->s_name);
	nRetVal = xnInitFromXmlFile(s->s_name, &(x->pContext), pErrors);

	if (nRetVal == XN_STATUS_NO_NODE_PRESENT)
	{
		XnChar strError[1024];
		xnEnumerationErrorsToString(pErrors, strError, 1024);
		LOG_ERROR2("XML config initialization failed", strError);
		xnEnumerationErrorsFree(pErrors);
		return;
	}
	xnEnumerationErrorsFree(pErrors);
	if (nRetVal != XN_STATUS_OK)
	{
		CHECK_RC_ERROR_EXIT(nRetVal, "XML config initialization open failed");
	}
	
	nRetVal = xnFindExistingNodeByType(x->pContext, XN_NODE_TYPE_DEPTH, &(x->hDepth));
	CHECK_RC_WARN(nRetVal, "No DEPTH node created/found");
	nRetVal = xnFindExistingNodeByType(x->pContext, XN_NODE_TYPE_IMAGE, &(x->hImage));
	CHECK_RC_WARN(nRetVal, "No IMAGE node created/found");
	if (!(x->hDepth) && !(x->hImage))
	{
		LOG_ERROR("Must have at least one DEPTH or IMAGE generator to function correctly");
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
		xnGetDepthMetaData(x->hDepth, x->pDepthMD);
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
		xnGetImageMetaData(x->hImage, x->pImageMD);
	}

	object_post((t_object*)x, "==Current active modes==");
	if (x->pDepthMD) object_post((t_object*)x, "DepthMD FPS=%lu X=%lu Y=%lu Z=%u", x->pDepthMD->pMap->nFPS, x->pDepthMD->pMap->FullRes.X, x->pDepthMD->pMap->FullRes.Y, x->pDepthMD->nZRes);
	if (x->pImageMD) object_post((t_object*)x, "ImageMD FPS=%lu X=%lu Y=%lu", x->pImageMD->pMap->nFPS, x->pImageMD->pMap->FullRes.X, x->pImageMD->pMap->FullRes.Y);
#endif
}




