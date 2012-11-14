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

#ifdef __cplusplus
	#ifdef _MSC_VER
		#pragma message("Warning - C compiler needed, this is being erroroneously compiled with C++")
	#else
		#warning Warning - C compiler needed, this is being erroroneously compiled with C++
	#endif
#endif

//---------------------------------------------------------------------------
// includes
//---------------------------------------------------------------------------

#ifdef WIN_VERSION
	#include "targetver.h"
	#include <direct.h>
#endif

#include "jit.common.h"		// has to be before ext.h due to deep header Max 6.0.4 SDK dependency bug regarding quicktime macro detection
#include "ext.h"
#include "ext_obex.h"
#include "max.jit.mop.h"
#include "jpatcher_api.h"
#include "XnOpenNI.h"
#include "XnVNiteVersion.h"

//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------

#define JIT_OPENNI_VERSION_MAJOR 0
#define JIT_OPENNI_VERSION_MINOR 8
#define JIT_OPENNI_VERSION_INCRM 6
#define JIT_OPENNI_VERSION "v" \
	XN_STRINGIFY(JIT_OPENNI_VERSION_MAJOR) "." \
	XN_STRINGIFY(JIT_OPENNI_VERSION_MINOR) "." \
	XN_STRINGIFY(JIT_OPENNI_VERSION_INCRM)
//#define OPENNI_REQUIRED_VERSION 103020003	// This is the XN_VERSION (or greater) required for this codebase
#define OPENNI_REQUIRED_VERSION XN_VERSION	// This is the XN_VERSION (or greater) required for this codebase
#define MAX_NUM_USERS_SUPPORTED 15		// no known OpenNI API to determine the max number possible for user generators
#define NUM_OF_SKELETON_JOINT_TYPES 24	// no known OpenNI API to determine the number of joints types (head, left foot, etc.) for a user generator
#define NUM_OPENNI_GENERATORS 5
#define DEPTH_GEN_INDEX 0
#define IMAGE_GEN_INDEX 1
#define IR_GEN_INDEX 2
#define USER_GEN_INDEX 3
#define SCENE_GEN_INDEX 4

#define NUM_OPENNI_MAPS 4		// total number of matrices outputs/outlets derived from OpenNI maps
#define DEPTHMAP_OUTPUT_INDEX 0	// currently all map generators MUST be first in order
#define IMAGEMAP_OUTPUT_INDEX 1
#define IRMAP_OUTPUT_INDEX 2
#define USERPIXELMAP_OUTPUT_INDEX 3

#define NUM_OPENNI_MISC_OUTPUTS 1	// this should be number of outputs that are not matrices from OpenNI maps and not dumpout (e.g. skeleton output)
#define SKELETON_OUTPUT_INDEX 4

#define DEPTHMAP_ASSIST_TEXT "(matrix) depthmap generator"	// if adding or removing assist text definitions, update max_jit_openni_assist() in max.jit.openni.c
#define IMAGEMAP_ASSIST_TEXT "(matrix) imagemap generator"
#define IRMAP_ASSIST_TEXT "(matrix) irmap generator"
#define USERPIXELMAP_ASSIST_TEXT "(matrix) usermap generator"
#define SKELETON_ASSIST_TEXT "(message) skeletons, floor info"

#define NUM_JITOPENNI_OUTPUTS (NUM_OPENNI_MAPS + NUM_OPENNI_MISC_OUTPUTS)	// this is the total number of outputs on the jit.open external not counting dumpout.

#define MAKEULONGFROMCHARS(a, b, c, d) ((unsigned long)((unsigned long)a | ((unsigned long)b << 8) | ((unsigned long)c << 16) | ((unsigned long)d << 24)))

#if defined(_MSC_VER) && defined(WIN_VERSION)	// Visual Studio on Windows
	#define LOG_ERROR(what, ...)	object_error((t_object*)x, what, __VA_ARGS__)
#else
	#define LOG_ERROR(what, ...)	object_error((t_object*)x, what, ##__VA_ARGS__)
#endif

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

#define LOG_WARNING3(what, param1, param2)									\
	{																		\
		object_warn((t_object*)x, what, param1, param2);					\
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


// Max 6.0.7 or earlier cpost() fails when run on OSx 10.8.x; it is a known Max/OS issue
// the fix, is to run a newer Max version on debug builds or an older OSx
// the workaround is to define CPOSTOFF in your preprocessor build settings
#ifdef _DEBUG																
	#if defined(_MSC_VER) && defined(WIN_VERSION)	// Visual Studio on Windows
		#if defined(CPOSTOFF)
			#define LOG_DEBUG(what, ...)	post(what, __VA_ARGS__)
		#else
			#define LOG_DEBUG(what, ...)	cpost(what, __VA_ARGS__)
		#endif
	#else
		#if defined(CPOSTOFF)
			#define LOG_DEBUG(what, ...)	post(what, ##__VA_ARGS__)
		#else
			#define LOG_DEBUG(what, ...)	cpost(what, ##__VA_ARGS__)
		#endif
	#endif
#else
	#define LOG_DEBUG(what, ...)
#endif

#define CHECK_RC_ERROR_EXIT(rc, what)										\
	if (rc != XN_STATUS_OK)													\
	{																		\
		object_error((t_object*)x, "%s (%s)", what, xnGetStatusString(rc));	\
		return;																\
	}

//---------------------------------------------------------------------------
// typedefs and enums
//---------------------------------------------------------------------------

#if !defined(BOOLEAN)
	#if defined(__BOOL_DEFINED)
		#define BOOLEAN bool
	#else
		#define BOOLEAN unsigned char
	#endif
#endif
#define boolean BOOLEAN

// Max object instance data
// Note: most instance data is in the Jitter object which we will wrap
typedef struct _max_jit_openni {
	t_object	ob;
	void		*obex;
	t_object	*osc_outlet;
	void		*pRegistrationForEvents;
	unsigned char		chrSkeletonOutputFormat;
} t_max_jit_openni;

// enum JitOpenNIEvents must match the number/order of friendly string array defined in max.jit.openni.c
enum JitOpenNIEvents {	JITOPENNI_NEW_USER,
						JITOPENNI_LOST_USER,
						JITOPENNI_CALIB_POSE_DETECTED,
						JITOPENNI_CALIB_START,
						JITOPENNI_CALIB_SUCCESS,
						JITOPENNI_CALIB_FAIL,
						JITOPENNI_EXIT_USER,
						JITOPENNI_REENTER_USER
};

typedef struct user_and_joints {
	XnBool bUserSkeletonTracked;
	XnUserID userID;
	XnSkeletonJointTransformation jointTransform[NUM_OF_SKELETON_JOINT_TYPES];
	XnPoint3D userCoM;
} t_user_and_joints;

// Our Jitter object instance data
typedef struct _jit_openni {
	t_object	ob;
	void *pParent;
	XnContext *pContext;
	XnNodeHandle hScriptNode;	// this will own the nodes created through loading an XML config file
	XnNodeHandle hProductionNode[NUM_OPENNI_GENERATORS]; // this only holds production node GENERATORS
	XnDepthMetaData *pMapMetaData[NUM_OPENNI_MAPS];
	XnUserID aUserIDs[MAX_NUM_USERS_SUPPORTED];
	XnCallbackHandle hUserCallbacks, hCalibrationStartCallback, hCalibrationCompleteCallback, hPoseCallbacks, hUserExitCallback, hUserReEnterCallback;
	XnChar strRequiredCalibrationPose[XN_MAX_NAME_LENGTH];
	float fPositionConfidenceFilter, fOrientConfidenceFilter, fSkeletonSmoothingFactor;
	char bOutputSkeletonOrientation, bOutputDepthmap, bOutputImagemap, bOutputIRmap, bOutputUserPixelsmap, bOutputSkeleton, siSkeletonValueType, bOutputSceneFloor;
	char siSkeletonProfile;
	char cbDistInMeters;
	XnUInt16 iNumUsersSeen;
	t_user_and_joints *pUserSkeletonJoints;
	XnPlane3D planeFloor;
	t_jit_linklist *pEventCallbackFunctions;
	BOOLEAN bHaveValidGeneratorProductionNode, bNeedPose, bHaveSkeletonSupport;
} t_jit_openni;

typedef struct _jit_openni_ndim {
	XnMapMetaData	*pMap;
	XnDepthPixel	*pData;		// in code, this is cast to the needed pixel type
	char			cbDistInMeters;
	char			cbIsDepthData;
} t_jit_openni_ndim;

typedef void (* JitOpenNIEventHandler)(t_jit_openni *x, enum JitOpenNIEvents iEventType, XnUserID userID);

//---------------------------------------------------------------------------
// Prototypes
//---------------------------------------------------------------------------

// jit.openni.c
BEGIN_USING_C_LINKAGE
t_jit_err		jit_openni_init(void); 
t_jit_openni	*jit_openni_new(void *pParent);
void			jit_openni_free					(t_jit_openni *x);
t_jit_err		jit_openni_matrix_calc			(t_jit_openni *x, void *inputs, void *outputs);
void			jit_openni_calculate_ndim		(t_jit_openni_ndim *ndim_holder, long dimcount, long *dim, long planecount, t_jit_matrix_info *minfo1, char *bp1, t_jit_parallel_ndim_worker *para_worker);
END_USING_C_LINKAGE
void			jit_openni_release_script_resources(t_jit_openni *x);
t_jit_err		jit_openni_skelsmooth_set		(t_jit_openni *x, void *attr, long ac, t_atom *av);
void			jit_openni_init_from_xml		(t_jit_openni *x, t_symbol *s, XnStatus *nRetVal);
t_jit_err		changeMatrixOutputGivenMapMetaData(void *pMetaData, t_jit_matrix_info *pMatrixOut);
void			copy16BitDatatoJitterMatrix		(t_jit_openni_ndim *ndim_holder, long dimcount, long *dim, long planecount, t_jit_matrix_info *minfo1, char *bp1, long rowOffset);
t_jit_err RegisterJitOpenNIEventCallbacks		(t_jit_openni *x, JitOpenNIEventHandler funcCallback, void **pUnregister);
t_jit_err UnregisterJitOpenNIEventCallbacks		(t_jit_openni *x, void *pUnregister);
void XN_CALLBACK_TYPE User_NewUser				(XnNodeHandle hUserGenerator, XnUserID userID, t_jit_openni *x);
void XN_CALLBACK_TYPE User_LostUser				(XnNodeHandle hUserGenerator, XnUserID userID, t_jit_openni *x);
void XN_CALLBACK_TYPE UserPose_PoseDetected	(XnNodeHandle hPoseCapability, const XnChar *strPose, XnUserID userID, t_jit_openni *x);
void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(XnNodeHandle hSkeletonCapability, XnUserID userID, t_jit_openni *x);
void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(XnNodeHandle hSkeletonCapability, XnUserID userID, XnCalibrationStatus calibrationResult, t_jit_openni *x);
void XN_CALLBACK_TYPE User_Exit					(XnNodeHandle hUserGenerator, XnUserID userID, t_jit_openni *x);
void XN_CALLBACK_TYPE User_ReEnter				(XnNodeHandle hUserGenerator, XnUserID userID, t_jit_openni *x);
t_jit_err jit_openni_depthfov_get				(t_jit_openni *x, void *attr, long *ac, t_atom **av);
t_jit_err jit_openni_scenefloor_get				(t_jit_openni *x, void *attr, long *ac, t_atom **av);

// max.jit.openni.c
BEGIN_USING_C_LINKAGE
void			*max_jit_openni_new				(t_symbol *s, long argc, t_atom *argv);
void			max_jit_openni_free				(t_max_jit_openni *x);
void			max_jit_openni_outputmatrix		(t_max_jit_openni *x);
void			max_jit_openni_assist			(t_max_jit_openni *x, void *b, long io, long index, char *s);
END_USING_C_LINKAGE
void			max_jit_openni_XMLConfig_read	(t_max_jit_openni *x, t_symbol *s, short argc, t_atom *argv);
void			max_jit_openni_post_events		(t_jit_openni *x, enum JitOpenNIEvents iEventType, XnUserID userID);
void			max_jit_openni_get_versions		(t_max_jit_openni *x, t_symbol *s, short argc, t_atom *argv);

#endif
