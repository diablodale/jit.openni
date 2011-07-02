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

#ifndef ADD_H_JIT_OPENNI
#define ADD_H_JIT_OPENNI

//---------------------------------------------------------------------------
// includes
//---------------------------------------------------------------------------

// max.jit.openni.c
#include "ext.h"
#include "ext_obex.h"
#include "jit.common.h"
#include "max.jit.mop.h"

// jit.openni.c
//#include "jit.common.h"
#include "XnOpenNI.h"

//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------

#define JIT_OPENNI_VERSION "v0.5.0"
#define NUM_OPENNI_GENERATORS 4
#define DEPTH_GEN_INDEX 0	// BUGBUG currently all map generators MUST be first in order
#define IMAGE_GEN_INDEX 1
#define IR_GEN_INDEX 2
#define USER_GEN_INDEX 3

#define MAKEULONGFROMCHARS(a, b, c, d) ((unsigned long)((unsigned long)a | ((unsigned long)b << 8) | ((unsigned long)c << 16) | ((unsigned long)d << 24)))

#define LOG_ERROR(what)														\
	{																		\
		object_error((t_object*)x, what);									\
	}

#define LOG_ERROR2(what, errorstring)										\
	{																		\
		object_error((t_object*)x, "%s (%s)", what, errorstring);			\
	}

#define LOG_WARNING(what)													\
	{																		\
		object_warn((t_object*)x, what);									\
	}

#define LOG_WARNING2(what, param1)											\
	{																		\
		object_warn((t_object*)x, what, param1);							\
	}

#define LOG_WARNING_RC(rc, what)											\
	{																		\
	object_warn((t_object*)x, "%s (%s)", what, xnGetStatusString(rc));		\
	}

#define LOG_COMMENT(what)													\
	{																		\
		object_post((t_object*)x, what);									\
	}

#define LOG_COMMENT2(what, param1)											\
	{																		\
		object_post((t_object*)x, what, param1);							\
	}

#define LOG_COMMENT3(what, param1, param2)									\
	{																		\
		object_post((t_object*)x, what, param1, param2);					\
	}

#ifdef _DEBUG																
	#define LOG_DEBUG(what) LOG_COMMENT(what)
	#define LOG_DEBUG2(what,param1) LOG_COMMENT2(what,param1)
	#define LOG_DEBUG3(what,param1,param2) LOG_COMMENT3(what,param1,param2)
#else
	#define LOG_DEBUG(what)
	#define LOG_DEBUG2(what,param1)
	#define LOG_DEBUG3(what,param1,param2)
#endif

#define CHECK_RC_ERROR_EXIT(rc, what)										\
	if (rc != XN_STATUS_OK)													\
	{																		\
		object_error((t_object*)x, "%s (%s)", what, xnGetStatusString(rc));	\
		return;																\
	}

//---------------------------------------------------------------------------
// typedefs
//---------------------------------------------------------------------------

// Max object instance data
// Note: most instance data is in the Jitter object which we will wrap
typedef struct _max_jit_openni {
	t_object	ob;
	void		*obex;
} t_max_jit_openni;


// Our Jitter object instance data
typedef struct _jit_openni {
	t_object	ob;
	XnContext* pContext;
	XnNodeHandle hProductionNode[NUM_OPENNI_GENERATORS]; // this only holds production node GENERATORS!
	XnNodeInfoList* pProductionNodeList;
	boolean bHaveValidGeneratorProductionNode;
	XnMapMetaData *pMapMetaData[NUM_OPENNI_GENERATORS];	// really only need the number of map generators
	XnCallbackHandle hUserCallbacks, hCalibrationCallbacks, hPoseCallbacks;
	XnBool bNeedPose;
	XnChar strRequiredCalibrationPose[20];
} t_jit_openni;


//---------------------------------------------------------------------------
// Prototypes
//---------------------------------------------------------------------------

// jit.openni.c
t_jit_err		jit_openni_init(void); 
t_jit_openni	*jit_openni_new(void);
void			jit_openni_free					(t_jit_openni *x);
void			jit_openni_init_from_xml		(t_jit_openni *x, t_symbol *s); // TODO should this return a t_jit_err?
t_jit_err		jit_openni_matrix_calc			(t_jit_openni *x, void *inputs, void *outputs);
void			copy16BitDatatoJitterMatrix		(XnDepthMetaData *pMapMetaData, long dimcount, long *dim, long planecount, t_jit_matrix_info *minfo1, char *bp1, long rowOffset);
void			max_jit_openni_assist			(t_max_jit_openni *x, void *b, long io, long index, char *s);
void			jit_openni_calculate_ndim		(XnDepthMetaData *pMapMetaData, long dimcount, long *dim, long planecount, t_jit_matrix_info *minfo1, char *bp1, t_jit_parallel_ndim_worker *para_worker);
void __stdcall	User_NewUser					(XnNodeHandle hUserGenerator, XnUserID userID, t_jit_openni *x);
void __stdcall	User_LostUser					(XnNodeHandle hUserGenerator, XnUserID userID, t_jit_openni *x);
void __stdcall UserPose_PoseDetected	(XnNodeHandle hPoseCapability, const XnChar *strPose, XnUserID userID, t_jit_openni *x);
void __stdcall UserCalibration_CalibrationStart(XnNodeHandle hSkeletonCapability, XnUserID userID, t_jit_openni *x);
void __stdcall UserCalibration_CalibrationEnd	(XnNodeHandle hSkeletonCapability, XnUserID userID, XnBool bSuccess, t_jit_openni *x);

// max.jit.openni.c
t_jit_err		jit_openni_init					(void);
void			*max_jit_openni_new				(t_symbol *s, long argc, t_atom *argv);
void			max_jit_openni_free				(t_max_jit_openni *x);
void			max_jit_openni_XMLConfig_read	(t_max_jit_openni *x, t_symbol *s, short argc, t_atom *argv);
void			max_jit_openni_outputmatrix		(t_max_jit_openni *x);


#endif