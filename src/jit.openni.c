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
	t_jit_object	*attr, *mop;
	void			*output;
	t_atom			a_arr[4];
	int				i;

	s_jit_openni_class = jit_class_new("jit_openni", (method)jit_openni_new, (method)jit_openni_free, sizeof(t_jit_openni), A_OBJ, 0);

	// add matrix operator (mop)
	mop = (t_jit_object*)jit_object_new(_jit_sym_jit_mop, 0, NUM_OPENNI_MAPS); // no matrix inputs, matrix outputs (generator outputs) + default dumpout

#ifdef DEPTHMAP_OUTPUT_INDEX
	//jit_mop_output_nolink(mop, DEPTHMAP_OUTPUT_INDEX + 1);				// if I nolink() then I can't change the attributes in the inspector
	output = jit_object_method(mop, _jit_sym_getoutput, DEPTHMAP_OUTPUT_INDEX + 1);
	jit_attr_setlong(output,_jit_sym_minplanecount,1);
	jit_attr_setlong(output,_jit_sym_maxplanecount,1);
	jit_attr_setlong(output,_jit_sym_mindimcount,2);
	jit_attr_setlong(output,_jit_sym_maxdimcount,2);
	jit_atom_setsym(&a_arr[0], _jit_sym_long);						// set to be the default
	jit_atom_setsym(&a_arr[1], _jit_sym_float32);
	jit_atom_setsym(&a_arr[2], _jit_sym_float64);
	jit_object_method(output,_jit_sym_types,3,a_arr);
#endif

#ifdef IMAGEMAP_OUTPUT_INDEX
	jit_mop_output_nolink(mop, IMAGEMAP_OUTPUT_INDEX + 1);
	output = jit_object_method(mop, _jit_sym_getoutput, IMAGEMAP_OUTPUT_INDEX + 1);
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

#ifdef USERPIXELMAP_OUTPUT_INDEX
	jit_mop_output_nolink(mop, USERPIXELMAP_OUTPUT_INDEX + 1);
	output = jit_object_method(mop,_jit_sym_getoutput, USERPIXELMAP_OUTPUT_INDEX + 1);
	jit_attr_setlong(output,_jit_sym_minplanecount,1);
	jit_attr_setlong(output,_jit_sym_maxplanecount,1);
	jit_attr_setlong(output,_jit_sym_mindimcount,2);
	jit_attr_setlong(output,_jit_sym_maxdimcount,2);
	jit_atom_setsym(&a_arr[0], _jit_sym_long);					// set to be the default
	jit_atom_setsym(&a_arr[1], _jit_sym_float32);
	jit_atom_setsym(&a_arr[2], _jit_sym_float64);
	jit_object_method(output,_jit_sym_types,3,a_arr);
#endif

#ifdef IRMAP_OUTPUT_INDEX
	jit_mop_output_nolink(mop, IRMAP_OUTPUT_INDEX + 1);
	output = jit_object_method(mop,_jit_sym_getoutput, IRMAP_OUTPUT_INDEX + 1);
	jit_attr_setlong(output,_jit_sym_minplanecount,1);
	jit_attr_setlong(output,_jit_sym_maxplanecount,1);
	jit_attr_setlong(output,_jit_sym_mindimcount,2);
	jit_attr_setlong(output,_jit_sym_maxdimcount,2);
	jit_atom_setsym(&a_arr[0], _jit_sym_long);					// set to be the default
	jit_atom_setsym(&a_arr[1], _jit_sym_float32);
	jit_atom_setsym(&a_arr[2], _jit_sym_float64);
	jit_object_method(output,_jit_sym_types,3,a_arr);
#endif

	jit_class_addadornment(s_jit_openni_class, mop);

	// add method(s)
	jit_class_addmethod(s_jit_openni_class, (method)jit_openni_matrix_calc, "matrix_calc", A_CANT, 0);
	jit_class_addmethod(s_jit_openni_class, (method)jit_openni_init_from_xml, "init_from_xml", A_SYM, 0);

	// add attribute(s)
	attr = jit_object_new(_jit_sym_jit_attr_offset, "position_confidence_filter", _jit_sym_float32, JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW,
			NULL, NULL, calcoffset(t_jit_openni, fPositionConfidenceFilter));
	jit_attr_addfilterset_clip(attr,0.0,1.0,TRUE,TRUE);
	jit_class_addattr(s_jit_openni_class, attr);

	attr = jit_object_new(_jit_sym_jit_attr_offset, "orient_confidence_filter", _jit_sym_float32, JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW,
			NULL, NULL, calcoffset(t_jit_openni, fOrientConfidenceFilter));
	jit_attr_addfilterset_clip(attr,0.0,1.0,TRUE,TRUE);
	jit_class_addattr(s_jit_openni_class, attr);
	
	attr = jit_object_new(_jit_sym_jit_attr_offset, "output_skeleton_orientation", _jit_sym_char, JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW,
			NULL, NULL, calcoffset(t_jit_openni, bOutputSkeletonOrientation));
	jit_attr_addfilterset_clip(attr,0,1,TRUE,TRUE);
	jit_class_addattr(s_jit_openni_class, attr);
	
	attr = jit_object_new(_jit_sym_jit_attr_offset, "skeleton_smooth_factor", _jit_sym_float32, JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW,
			NULL, jit_openni_skelsmooth_set, calcoffset(t_jit_openni, fSkeletonSmoothingFactor));
	jit_attr_addfilterset_clip(attr,0.0,1.0,TRUE,TRUE);
	jit_class_addattr(s_jit_openni_class, attr);

	attr = jit_object_new(_jit_sym_jit_attr_offset, "output_depthmap", _jit_sym_char, JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW,
			NULL, NULL, calcoffset(t_jit_openni, bOutputDepthmap));
	jit_attr_addfilterset_clip(attr,0,1,TRUE,TRUE);
	jit_class_addattr(s_jit_openni_class, attr);

	attr = jit_object_new(_jit_sym_jit_attr_offset, "output_imagemap", _jit_sym_char, JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW,
			NULL, NULL, calcoffset(t_jit_openni, bOutputImagemap));
	jit_attr_addfilterset_clip(attr,0,1,TRUE,TRUE);
	jit_class_addattr(s_jit_openni_class, attr);

	attr = jit_object_new(_jit_sym_jit_attr_offset, "output_irmap", _jit_sym_char, JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW,
			NULL, NULL, calcoffset(t_jit_openni, bOutputIRmap));
	jit_attr_addfilterset_clip(attr,0,1,TRUE,TRUE);
	jit_class_addattr(s_jit_openni_class, attr);

	attr = jit_object_new(_jit_sym_jit_attr_offset, "output_usermap", _jit_sym_char, JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW,
			NULL, NULL, calcoffset(t_jit_openni, bOutputUserPixelsmap));
	jit_attr_addfilterset_clip(attr,0,1,TRUE,TRUE);
	jit_class_addattr(s_jit_openni_class, attr);

	attr = jit_object_new(_jit_sym_jit_attr_offset, "output_skeleton", _jit_sym_char, JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW,
			NULL, NULL, calcoffset(t_jit_openni, bOutputSkeleton));
	jit_attr_addfilterset_clip(attr,0,1,TRUE,TRUE);
	jit_class_addattr(s_jit_openni_class, attr);

	attr = jit_object_new(_jit_sym_jit_attr_offset, "skeleton_value_type", _jit_sym_char, JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW,
			NULL, NULL, calcoffset(t_jit_openni, siSkeletonValueType));
	jit_attr_addfilterset_clip(attr,0,2,TRUE,TRUE);
	jit_class_addattr(s_jit_openni_class, attr);

	attr = jit_object_new(_jit_sym_jit_attr_offset_array, "depth_fov", _jit_sym_float32, 2, JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW | JIT_ATTR_SET_OPAQUE | JIT_ATTR_SET_OPAQUE_USER,
			(method)jit_openni_depthfov_get, NULL, NULL);
	jit_class_addattr(s_jit_openni_class, attr);

	attr = jit_object_new(_jit_sym_jit_attr_offset, "output_scene_floor", _jit_sym_char, JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW,
			NULL, NULL, calcoffset(t_jit_openni, bOutputSceneFloor));
	jit_attr_addfilterset_clip(attr,0,1,TRUE,TRUE);
	jit_class_addattr(s_jit_openni_class, attr);
	
	attr = jit_object_new(_jit_sym_jit_attr_offset_array, "scene_floor", _jit_sym_float32, 6, JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW | JIT_ATTR_SET_OPAQUE | JIT_ATTR_SET_OPAQUE_USER,
			(method)jit_openni_scenefloor_get, NULL, NULL);
	jit_class_addattr(s_jit_openni_class, attr);

	// TODO expose skeleton profile selection attribute
	//attr = jit_object_new(_jit_sym_jit_attr_offset, "skeleton_profile", _jit_sym_char, JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW,
	//		NULL, NULL, calcoffset(t_jit_openni, siSkeletonProfile));
	//jit_attr_addfilterset_clip(attr,1,5,TRUE,TRUE);
	//jit_class_addattr(s_jit_openni_class, attr);



	// finalize class
	jit_class_register(s_jit_openni_class);
	return JIT_ERR_NONE;
}


/************************************************************************************/
// Object Life Cycle

t_jit_openni *jit_openni_new(void *pParent)
{
	t_jit_openni	*x = NULL;
	
	x = (t_jit_openni*)jit_object_alloc(s_jit_openni_class);
	if (x)
	{
		XnStatus nRetVal = XN_STATUS_OK;
		x->pParent = pParent;
		x->bHaveValidGeneratorProductionNode = false;
		x->fPositionConfidenceFilter = 0.0;
		x->fOrientConfidenceFilter = 0.0;
		x->bOutputSkeletonOrientation = 0;
		x->bOutputDepthmap = 1;
		x->bOutputImagemap = 1;
		x->bOutputIRmap = 1;
		x->bOutputUserPixelsmap = 1;
		x->bOutputSkeleton = 1;
		x->bOutputSceneFloor = 0;
		x->fSkeletonSmoothingFactor = 0.0;	//BUGBUG what is the OpenNI/NITE default?
		x->siSkeletonValueType = 0;
		x->siSkeletonProfile = XN_SKEL_PROFILE_ALL;	//  XN_SKEL_PROFILE_ALL = 2
		x->pEventCallbackFunctions = (t_jit_linklist *)jit_object_new(_jit_sym_jit_linklist);

		LOG_DEBUG("Creating a new OpenNI context");
		if (nRetVal = xnInit(&(x->pContext)))
		{
			LOG_ERROR("jit_openni_new: cannot create new OpenNI context");
		}
		else
		{
			x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX] = (XnMapMetaData *)xnAllocateDepthMetaData();
			x->pMapMetaData[IMAGEMAP_OUTPUT_INDEX] = (XnMapMetaData *)xnAllocateImageMetaData();
			x->pMapMetaData[IRMAP_OUTPUT_INDEX] = (XnMapMetaData *)xnAllocateIRMetaData();
			x->pMapMetaData[USERPIXELMAP_OUTPUT_INDEX] = (XnMapMetaData *)xnAllocateSceneMetaData();
		}
		LOG_DEBUG("object created");
	}
	return x;
}

void jit_openni_release_script_resources(t_jit_openni *x)
{
	int i;

	if (x->hUserCallbacks)
	{
		xnUnregisterUserCallbacks(x->hProductionNode[USER_GEN_INDEX], x->hUserCallbacks);
		x->hUserCallbacks = NULL;
	}
	if (x->hPoseCallbacks)
	{
		xnUnregisterFromPoseDetected(x->hProductionNode[USER_GEN_INDEX], x->hPoseCallbacks);
		x->hPoseCallbacks = NULL;
	}
	if (x->hCalibrationStartCallback)
	{
		xnUnregisterFromCalibrationStart(x->hProductionNode[USER_GEN_INDEX], x->hCalibrationStartCallback);
		x->hCalibrationStartCallback = NULL;
	}
	if (x->hCalibrationCompleteCallback)
	{
		xnUnregisterFromCalibrationComplete(x->hProductionNode[USER_GEN_INDEX], x->hCalibrationCompleteCallback);
		x->hCalibrationCompleteCallback = NULL;
	}
	if (x->hUserExitCallback)
	{
		xnUnregisterFromUserExit(x->hProductionNode[USER_GEN_INDEX], x->hUserExitCallback);
		x->hUserExitCallback = NULL;
	}
	if (x->hUserReEnterCallback)
	{
		xnUnregisterFromUserReEnter(x->hProductionNode[USER_GEN_INDEX], x->hUserReEnterCallback);
		x->hUserReEnterCallback = NULL;
	}
	if (x->pUserSkeletonJoints)
	{
		sysmem_freeptr(x->pUserSkeletonJoints);		//BUGBUG this is not thread safe, matrix_calc could be running elsewhere
		x->pUserSkeletonJoints = NULL;
	}
	for (i=0; i<NUM_OPENNI_GENERATORS; i++)			// TODO consider moving away from the node array below and use xnEnumerateExistingNodes() instead
	{
		if (x->hProductionNode[i])
		{
			xnProductionNodeRelease(x->hProductionNode[i]);	// due to xnNodeInfoGetRefHandle() have to free them here
			x->hProductionNode[i] = NULL;
		}
	}
	if (x->hScriptNode)
	{
		xnProductionNodeRelease(x->hScriptNode);			//BUGBUG this is not thread safe, matrix_calc could be running elsewhere
		x->hScriptNode = NULL;
	}
	LOG_DBGVIEW("Just released script resources");
}

void jit_openni_free(t_jit_openni *x)
{
	jit_object_free(x->pEventCallbackFunctions);
	if (x->pContext)
	{
		jit_openni_release_script_resources(x);
		xnFreeDepthMetaData((XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX]);
		xnFreeImageMetaData((XnImageMetaData *)x->pMapMetaData[IMAGEMAP_OUTPUT_INDEX]);
		xnFreeIRMetaData((XnIRMetaData *)x->pMapMetaData[IRMAP_OUTPUT_INDEX]);
		xnFreeSceneMetaData((XnSceneMetaData *)x->pMapMetaData[USERPIXELMAP_OUTPUT_INDEX]);
		LOG_DEBUG("Releasing OpenNI context...");
		xnContextRelease(x->pContext);
	}
}

t_jit_err jit_openni_skelsmooth_set(t_jit_openni *x, void *attr, long ac, t_atom *av)
{
	if (ac && av)
	{
		x->fSkeletonSmoothingFactor = jit_atom_getfloat(av);
	}
	else
	{
		// no args, set to zero
		x->fSkeletonSmoothingFactor = 0.0;
	}
	if (x->bHaveSkeletonSupport) xnSetSkeletonSmoothing(x->hProductionNode[USER_GEN_INDEX],x->fSkeletonSmoothingFactor);
	return JIT_ERR_NONE;
}

t_jit_err jit_openni_matrix_calc(t_jit_openni *x, void *inputs, void *outputs)
{
	t_jit_err err=JIT_ERR_NONE;
	long out_savelock[NUM_OPENNI_MAPS];
	t_jit_matrix_info out_minfo;
	char *out_bp;	// char* so can reference down to a single byte as needed
	int i, j, dimcount;
	void *out_matrix[NUM_OPENNI_MAPS];
	boolean bGotOutMatrices = true;
	XnStatus nRetVal = XN_STATUS_OK;
	XnUInt16 tmpNumUsers = MAX_NUM_USERS_SUPPORTED;
#ifdef _DEBUG
	long long unsigned lluTimestamp = 0;
	long unsigned luFrameID;
#endif

	if (!x->bHaveValidGeneratorProductionNode) return JIT_ERR_HW_UNAVAILABLE;
	
	// get the zeroth index input and output from
	// the corresponding input and output lists
	for (i=0;i<NUM_OPENNI_MAPS;i++)				// TODO could optimize this by using jitter linked list flattening API
	{
		if (!(out_matrix[i] = jit_object_method(outputs,_jit_sym_getindex,i)))
		{
			LOG_DEBUG2("could not get output [%d] matrix", i);
			bGotOutMatrices = false;
		}
	}

	// if the object and both input and output matrices, both generators are valid, then process else error
	if (x && bGotOutMatrices)
	{
		// lock input and output matrices TODO its possible to move this inside the below iteration through NUM_OPENNI_GENERATORS (for those that have outlets) if it is not important for all matrices to be locked before any work
		for (i = 0; i< NUM_OPENNI_MAPS; i++)
		{
			out_savelock[i] = (long) jit_object_method(out_matrix[i],_jit_sym_lock,1);
		}

		// Don't wait for new data, just update all generators with the newest already available
		nRetVal = xnWaitNoneUpdateAll(x->pContext);
		if (nRetVal != XN_STATUS_OK)
		{
			LOG_ERROR2("Failed updating generator nodes", xnGetStatusString(nRetVal));
			err = JIT_ERR_DATA_UNAVAILABLE;
		}
		else
		{
			LOG_DEBUG("updated generators and metadata, wait for none");
			for (i = 0; i< NUM_OPENNI_GENERATORS; i++)
			{
				if (x->hProductionNode[i])
				{
					// fill out matrix info structs for input and output
					if (i < NUM_OPENNI_MAPS) jit_object_method(out_matrix[i],_jit_sym_getinfo,&out_minfo);

					LOG_DEBUG3("generator[%d] type=%s", i, xnProductionNodeTypeToString(xnNodeInfoGetDescription(xnGetNodeInfo(x->hProductionNode[i]))->Type));
					LOG_DEBUG3("generator[%d] is derived from map=%s", i, xnIsTypeDerivedFrom(xnNodeInfoGetDescription(xnGetNodeInfo(x->hProductionNode[i]))->Type, XN_NODE_TYPE_MAP_GENERATOR) ? "true":"false");
					if (!xnIsDataNew(x->hProductionNode[i])) continue;
					switch(xnNodeInfoGetDescription(xnGetNodeInfo(x->hProductionNode[i]))->Type)
					{
					case XN_NODE_TYPE_DEPTH:
						if (!x->bOutputDepthmap) continue;
						xnGetDepthMetaData(x->hProductionNode[i], (XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX]);
						if (err = changeMatrixOutputGivenMapMetaData(x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX], &out_minfo)) goto out;
						break;
					case XN_NODE_TYPE_IMAGE:
						if (!x->bOutputImagemap) continue;
						xnGetImageMetaData(x->hProductionNode[i], (XnImageMetaData *)x->pMapMetaData[IMAGEMAP_OUTPUT_INDEX]);
						if (err = changeMatrixOutputGivenMapMetaData(x->pMapMetaData[IMAGEMAP_OUTPUT_INDEX], &out_minfo)) goto out;
						break;
					case XN_NODE_TYPE_IR:
						if (!x->bOutputIRmap) continue;
						xnGetIRMetaData(x->hProductionNode[i], (XnIRMetaData *)x->pMapMetaData[IRMAP_OUTPUT_INDEX]);
						if (err = changeMatrixOutputGivenMapMetaData(x->pMapMetaData[IRMAP_OUTPUT_INDEX], &out_minfo)) goto out;
						break;
					case XN_NODE_TYPE_SCENE:
						// no need to xnGetSceneMetaData()
						if (!x->bOutputSceneFloor) continue;
						xnGetFloor(x->hProductionNode[i], &(x->planeFloor));
						switch (x->siSkeletonValueType) // case 0 is the default native OpenNI values (real world)
						{
						case 1:		// the OpenNI projective coordinates
							xnConvertRealWorldToProjective(x->hProductionNode[DEPTH_GEN_INDEX], 1, &(x->planeFloor.ptPoint), &(x->planeFloor.ptPoint));
							break;
						case 2:		// legacy "normalized" OSCeleton values
							// I do not support the legacy OSCeleton multiplier and offset, if a dev is that advanced, they can do the simple conversion to this object's native output
							// also, the legacy OSCeleton "normalization" formula used here is the same as for OSCeleton joints. Note that OSCeleton has a legacy bug in that
							// the "normalization" formulas for center of mass and joints differ. Since OSCeleton doesn't support floor data at all, I can only guess that a user
							// might want to know when a foot joint is near the floor and therefore use the legacy OSCeleton joint normalization formula
							x->planeFloor.ptPoint.X = (1280 - x->planeFloor.ptPoint.X) / 2560;	// "Normalize" coords to 0..1 interval
							x->planeFloor.ptPoint.Y = (960 - x->planeFloor.ptPoint.Y) / 1920;	// "Normalize" coords to 0..1 interval
							x->planeFloor.ptPoint.Z = x->planeFloor.ptPoint.Z * 7.8125 / 10000;	// "Normalize" coords to 0..7.8125 interval
							break;
						}
						continue;	// go to next generator because everything after the switch is map metadata code
					case XN_NODE_TYPE_USER:
						// next line invalidates all old skeletons, if I move this outside the for loop, then no new OSC data would be sent if no new User Generator data
						x->iNumUsersSeen = 0;
						if (x->bOutputSkeleton)
						{
							xnGetUsers(x->hProductionNode[USER_GEN_INDEX], x->aUserIDs, &tmpNumUsers);
							LOG_DEBUG2("Current num of users=%d", tmpNumUsers);
							for (j=0; j<tmpNumUsers; j++)
							{
								int iJoint;

								LOG_DEBUG3("user[%d]=%d", j, x->aUserIDs[j]);
								x->pUserSkeletonJoints[j].userID = x->aUserIDs[j];
								x->pUserSkeletonJoints[j].bUserSkeletonTracked = xnIsSkeletonTracking(x->hProductionNode[USER_GEN_INDEX], x->aUserIDs[j]);
								xnGetUserCoM(x->hProductionNode[USER_GEN_INDEX], x->aUserIDs[j], &(x->pUserSkeletonJoints[j].userCoM));
								switch (x->siSkeletonValueType)  // case 0 is the default native OpenNI values (real world)
								{
								case 1:		// the OpenNI projective coordinates
									xnConvertRealWorldToProjective(x->hProductionNode[DEPTH_GEN_INDEX], 1, &(x->pUserSkeletonJoints[j].userCoM), &(x->pUserSkeletonJoints[j].userCoM));
									break;
								case 2:		// legacy "normalized" OSCeleton values
									// I do not support the legacy OSCeleton multiplier and offset, if a dev is that advanced, they can do the simple conversion to this object's native output
									// also, the "normalization" formula differs between here for CoM and the position; this is a legacy bug of OSCeleton so I replicate it here for compatibility
									x->pUserSkeletonJoints[j].userCoM.X = (1280 - x->pUserSkeletonJoints[j].userCoM.X) / 2560;
									x->pUserSkeletonJoints[j].userCoM.Y = (1280 - x->pUserSkeletonJoints[j].userCoM.Y) / 2560;
									x->pUserSkeletonJoints[j].userCoM.Z = x->pUserSkeletonJoints[j].userCoM.Z * 7.8125 / 10000;
									break;
								}
								if (x->bHaveSkeletonSupport && x->pUserSkeletonJoints[j].bUserSkeletonTracked)
								{
									// BUGBUG need to use a, to be created, list of active joints rather than assuming all 24 now that I support skeletonprofiles

									// fill in joint struct
									for (iJoint = 1; iJoint <= NUM_OF_SKELETON_JOINT_TYPES; iJoint++)
									{
										xnGetSkeletonJoint(x->hProductionNode[USER_GEN_INDEX], x->aUserIDs[j], (XnSkeletonJoint)iJoint, &(x->pUserSkeletonJoints[j].jointTransform[iJoint]));
										switch (x->siSkeletonValueType) // case 0 is the default native OpenNI values (real world)
										{
										case 1:		// the OpenNI projective coordinates
											xnConvertRealWorldToProjective(x->hProductionNode[DEPTH_GEN_INDEX], 1, &(x->pUserSkeletonJoints[j].jointTransform[iJoint].position.position), &(x->pUserSkeletonJoints[j].jointTransform[iJoint].position.position));
											break;
										case 2:		// legacy "normalized" OSCeleton values
											// I do not support the legacy OSCeleton multiplier and offset, if a dev is that advanced, they can do the simple conversion to this object's native output
											// also, the "normalization" formula differs between CoM and here for position; this is a legacy bug of OSCeleton so I replicate it here for compatibility
											x->pUserSkeletonJoints[j].jointTransform[iJoint].position.position.X = (1280 - x->pUserSkeletonJoints[j].jointTransform[iJoint].position.position.X) / 2560;	// "Normalize" coords to 0..1 interval
											x->pUserSkeletonJoints[j].jointTransform[iJoint].position.position.Y = (960 - x->pUserSkeletonJoints[j].jointTransform[iJoint].position.position.Y) / 1920;	// "Normalize" coords to 0..1 interval
											x->pUserSkeletonJoints[j].jointTransform[iJoint].position.position.Z = x->pUserSkeletonJoints[j].jointTransform[iJoint].position.position.Z * 7.8125 / 10000;	// "Normalize" coords to 0..7.8125 interval
											break;
										}
									}
								}
							}
							x->iNumUsersSeen = tmpNumUsers;	// BUGBUG this and the code directly above are not multithread safe due to changing pUserSkeletonJoints structure without a mutex
						}
						if (!x->bOutputUserPixelsmap) continue;
						xnGetUserPixels(x->hProductionNode[USER_GEN_INDEX], 0, (XnSceneMetaData *)x->pMapMetaData[USERPIXELMAP_OUTPUT_INDEX]);
						if (err = changeMatrixOutputGivenMapMetaData(x->pMapMetaData[USERPIXELMAP_OUTPUT_INDEX], &out_minfo)) goto out;
						LOG_DEBUG3("**USERPXL**FrameID=%lu Timestamp=%llu", ((XnSceneMetaData *)x->pMapMetaData[i])->pMap->pOutput->nFrameID, ((XnSceneMetaData *)x->pMapMetaData[i])->pMap->pOutput->nTimestamp);
					}
//					if (xnIsTypeDerivedFrom(xnNodeInfoGetDescription(xnGetNodeInfo(x->hProductionNode[i]))->Type, XN_NODE_TYPE_MAP_GENERATOR))
					LOG_DEBUG("updated metadata for incoming map generator frame");
					LOG_DEBUG3("generator[%d] pixelformat=%s", i, xnPixelFormatToString(((XnDepthMetaData *)x->pMapMetaData[i])->pMap->PixelFormat));
					LOG_DEBUG3("FrameID=%lu Timestamp=%llu", ((XnDepthMetaData *)x->pMapMetaData[i])->pMap->pOutput->nFrameID, ((XnDepthMetaData *)x->pMapMetaData[i])->pMap->pOutput->nTimestamp);
#ifdef _DEBUG
					if (!lluTimestamp)
					{
						lluTimestamp = ((XnDepthMetaData *)x->pMapMetaData[i])->pMap->pOutput->nTimestamp;
						luFrameID = ((XnDepthMetaData *)x->pMapMetaData[i])->pMap->pOutput->nFrameID;
					}
					else
					{
						if (lluTimestamp != ((XnDepthMetaData *)x->pMapMetaData[i])->pMap->pOutput->nTimestamp) LOG_ERROR("Timestamps are not the same for all generators within a single update");
						if (luFrameID != ((XnDepthMetaData *)x->pMapMetaData[i])->pMap->pOutput->nFrameID) LOG_ERROR("FrameIDs are not the same for all generators within a single update");
					}
#endif
					jit_object_method(out_matrix[i], _jit_sym_setinfo, &out_minfo);	// after testing with max_jit_mop_notify(), setting info that is the same as the existing matrix does not trigger the modify or rebuild matrix functions
					jit_object_method(out_matrix[i], _jit_sym_getinfo, &out_minfo);	// BUGBUG for some reason, I have to call this or Max crashes when you change matrix attributes via inspector
#ifdef _DEBUG
					// get dimensions/planecount
					dimcount = out_minfo.dimcount;
					for (j=0;j<dimcount;j++)
					{
						object_post((t_object*)x, "out%d dim[%d] = %d", i, j, out_minfo.dim[j]);
						object_post((t_object*)x, "out%d dimstride[%d] = %d", i, j, out_minfo.dimstride[j]);
					}
					LOG_DEBUG3("out%d planes = %d", i, out_minfo.planecount);
					LOG_DEBUG3("out%d type = %s", i, out_minfo.type->s_name);
#endif
					// get matrix data pointers
					jit_object_method(out_matrix[i],_jit_sym_getdata,&out_bp);
					if (!out_bp) { err=JIT_ERR_INVALID_OUTPUT; goto out;} // if data pointers are invalid, set error, and cleanup

					// manually copy OpenNI arrays to jitter matrix because jitter doesn't directly support them
					// by using the Jitter parallel functions
					jit_parallel_ndim_simplecalc1((method)jit_openni_calculate_ndim, x->pMapMetaData[i], out_minfo.dimcount, out_minfo.dim, out_minfo.planecount, &out_minfo, out_bp, 0);
				}
			}
		}
out:
		if (err) x->iNumUsersSeen = 0;		// invalidate skeletons if encountered an error
		// restore matrix lock state to previous value
		for (i = 0; i< NUM_OPENNI_MAPS; i++)
		{
			jit_object_method(out_matrix[i],_jit_sym_lock,out_savelock[i]);
		}
	}
	else
	{
		return JIT_ERR_INVALID_OUTPUT;
	}
	return err;
}

t_jit_err changeMatrixOutputGivenMapMetaData(void *pMetaData, t_jit_matrix_info *pMatrixOut)
{
	// setup the outputs to support the map's PixelFormat, assumes 1st parameter of XnDepthMetaData is superset of all map metadatas
	switch(((XnDepthMetaData *)pMetaData)->pMap->PixelFormat)
	{
		case XN_PIXEL_FORMAT_RGB24:
			pMatrixOut->type = _jit_sym_char;
			pMatrixOut->planecount = 4;
			pMatrixOut->dim[0] = ((XnDepthMetaData *)pMetaData)->pMap->FullRes.X;
			pMatrixOut->dim[1] = ((XnDepthMetaData *)pMetaData)->pMap->FullRes.Y;
			break;
		case XN_PIXEL_FORMAT_YUV422:
			pMatrixOut->type = _jit_sym_char;
			pMatrixOut->planecount = 4;
			pMatrixOut->dim[0] = ((XnDepthMetaData *)pMetaData)->pMap->FullRes.X / 2;	// trusting that X res will always be an even number
			pMatrixOut->dim[1] = ((XnDepthMetaData *)pMetaData)->pMap->FullRes.Y;
			break;
		case XN_PIXEL_FORMAT_GRAYSCALE_8_BIT:
			pMatrixOut->type = _jit_sym_char;
			pMatrixOut->planecount = 1;
			pMatrixOut->dim[0] = ((XnDepthMetaData *)pMetaData)->pMap->FullRes.X;
			pMatrixOut->dim[1] = ((XnDepthMetaData *)pMetaData)->pMap->FullRes.Y;
			break;
		case XN_PIXEL_FORMAT_GRAYSCALE_16_BIT:
			pMatrixOut->planecount = 1;
			pMatrixOut->dim[0] = ((XnDepthMetaData *)pMetaData)->pMap->FullRes.X;
			pMatrixOut->dim[1] = ((XnDepthMetaData *)pMetaData)->pMap->FullRes.Y;
			break;
		default:
			return JIT_ERR_MISMATCH_TYPE;
	}
	return JIT_ERR_NONE;
}

void copy16BitDatatoJitterMatrix(XnDepthMetaData *pMapMetaData, long dimcount, long *dim, long planecount, t_jit_matrix_info *minfo1, char *bp1, long rowOffset)
{
	// this function assumes all parameters are valid, and requires that all map metadata's passed via pMapMetaData have the same byte location for the data itself
	
	int i, j;
	XnUInt16 *p16BitData; // aka XnDepthPixel or 16bit greysacale xnImagePixel

	// this ->pData assumes XnDepthMetaData struct, and pointer arithmetic automatically jumps by 2 bytes
	p16BitData = (XnUInt16 *)pMapMetaData->pData + (rowOffset * pMapMetaData->pMap->FullRes.X);
	
	for(i=0; i < dim[1]; i++)  // for each row
	{
		for(j=0; j < dim[0]; j++)  // go across each column
		{
			if (minfo1->type == _jit_sym_long)
			{
				((unsigned long *)bp1)[j] = *p16BitData;
			}
			else if (minfo1->type == _jit_sym_float32)
			{
				((float *)bp1)[j] = *p16BitData;
			}
			else // it is _jit_sym_float64
			{
				((double *)bp1)[j] = *p16BitData;
			}
			p16BitData++;
		}
		bp1 += minfo1->dimstride[1];
	}
}

void jit_openni_calculate_ndim(XnDepthMetaData *pMapMetaData, long dimcount, long *dim, long planecount, t_jit_matrix_info *minfo1, char *bp1, t_jit_parallel_ndim_worker *para_worker)
{
	// this function assumes all parameters are valid, and requires that all map metadata's passed via pMapMetaData have the same byte location of the 	
	// uses the t_jit_parallel_ndim_worker undocumented functionality as posted in the formums at http://cycling74.com/forums/topic.php?id=24525
	int i, j;
	long rowOffset = para_worker->offset[1];

	if (pMapMetaData->pMap->PixelFormat != XN_PIXEL_FORMAT_GRAYSCALE_16_BIT)
	{
		XnUInt8 *pMapData;

		// this ->pData assumes XnDepthMetaData struct, and pointer arithmetic jumps only 1 bytes so need to multiply by PixelFormat size
		pMapData = (XnUInt8 *)pMapMetaData->pData + (rowOffset * (pMapMetaData->pMap->FullRes.X * xnGetBytesPerPixelForPixelFormat(pMapMetaData->pMap->PixelFormat)));

		for(i=0; i < dim[1]; i++) // for each row
		{
			for(j=0; j < dim[0]; j++)  // go across each column
			{
				switch(pMapMetaData->pMap->PixelFormat)
				{
					case XN_PIXEL_FORMAT_RGB24:
						((unsigned long *)bp1)[j] = MAKEULONGFROMCHARS(0xFF, pMapData[0], pMapData[1], pMapData[2]); // not tested on big endian systems
						pMapData += 3;
						break;
					case XN_PIXEL_FORMAT_YUV422:	// ordering is U, Y1, V, Y2; if I give up sources that are not 4-byte aligned, I could use Jitter matrix copying functions
						((unsigned long *)bp1)[j] = MAKEULONGFROMCHARS(pMapData[0], pMapData[1], pMapData[2], pMapData[3]); // not tested on big endian systems
						pMapData += 4;
						break;
					case XN_PIXEL_FORMAT_GRAYSCALE_8_BIT:	// if I give up sources that are not 4-bte aligned, I could use Jitter matrix copying functions
															// TODO add support for long, float32, float64 output matrices
						bp1[j] = *pMapData++;
						break;
					// case XN_PIXEL_FORMAT_GRAYSCALE_16_BIT is now handled below by calling a shared function copy16BitDatatoJitterMatrix()
				}
			}
			bp1 += minfo1->dimstride[1];
		}
	}
	else
	{
		copy16BitDatatoJitterMatrix(pMapMetaData, dimcount, dim, planecount, minfo1, bp1, rowOffset);
	}
}

void jit_openni_init_from_xml(t_jit_openni *x, t_symbol *s, XnStatus *nRetVal)
{
	XnEnumerationErrors* pErrors;
	XnNodeInfoListIterator pCurrentNode;
	XnNodeInfo* pProdNodeInfo;
	XnNodeInfoList* pProductionNodeList;

	// release all previous script generated resources
	jit_openni_release_script_resources(x);

	// prep for new XML config load
	x->bHaveValidGeneratorProductionNode = false;
	x->bNeedPose = false;
	x->bHaveSkeletonSupport = false;
	x->iNumUsersSeen = 0;

	*nRetVal = xnEnumerationErrorsAllocate(&pErrors);
	CHECK_RC_ERROR_EXIT(*nRetVal, "jit_openni_init_from_xml: cannot allocate errors object");

	// load new XML config file, BUGBUG repeatedly unloading and reloading a script with a USER node will eventually cause a crash
	*nRetVal = xnContextRunXmlScriptFromFileEx(x->pContext, s->s_name, pErrors, &(x->hScriptNode));

	if (*nRetVal == XN_STATUS_NO_NODE_PRESENT)
	{
		XnChar strError[1024];
		xnEnumerationErrorsToString(pErrors, strError, 1024);
		LOG_ERROR2("XMLconfig initialization failed", strError);
	}
	xnEnumerationErrorsFree(pErrors);
	if (*nRetVal != XN_STATUS_OK)
	{
		CHECK_RC_ERROR_EXIT(*nRetVal, "XML config initialization open failed");
	}
	LOG_DEBUG2("XMLconfig loaded: %s", s->s_name);

	*nRetVal = xnEnumerateExistingNodes(x->pContext,&pProductionNodeList);
	CHECK_RC_ERROR_EXIT(*nRetVal, "XMLconfig cannot enumerate existing production nodes");
	for (pCurrentNode = xnNodeInfoListGetFirst(pProductionNodeList); xnNodeInfoListIteratorIsValid(pCurrentNode); pCurrentNode = xnNodeInfoListGetNext(pCurrentNode))
	{
		XnFieldOfView xFieldOfView;
		
		pProdNodeInfo = xnNodeInfoListGetCurrent(pCurrentNode);
		LOG_DEBUG2("found prodnode type=%s", xnProductionNodeTypeToString(xnNodeInfoGetDescription(pProdNodeInfo)->Type));
		LOG_DEBUG2("Derived from map=%s", xnIsTypeDerivedFrom(xnNodeInfoGetDescription(pProdNodeInfo)->Type, XN_NODE_TYPE_MAP_GENERATOR) ? "true":"false");

		switch(xnNodeInfoGetDescription(pProdNodeInfo)->Type)
		{
		case XN_NODE_TYPE_DEVICE:
		case XN_NODE_TYPE_SCRIPT:
			break;
		case XN_NODE_TYPE_DEPTH:
			x->hProductionNode[DEPTH_GEN_INDEX] = xnNodeInfoGetRefHandle(pProdNodeInfo);
			xnGetDepthMetaData(x->hProductionNode[DEPTH_GEN_INDEX], (XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX]);
			x->bHaveValidGeneratorProductionNode = true;
#ifdef _DEBUG
			xnGetDepthFieldOfView(x->hProductionNode[DEPTH_GEN_INDEX], &xFieldOfView);
			LOG_DEBUG3("Depth FOV Horz=%f Vert=%f", xFieldOfView.fHFOV, xFieldOfView.fVFOV);
			if (xnIsCapabilitySupported(x->hProductionNode[DEPTH_GEN_INDEX], XN_CAPABILITY_USER_POSITION))
			{
				unsigned int uUserPos, i;
				XnBoundingBox3D box;

				uUserPos = xnGetSupportedUserPositionsCount(x->hProductionNode[DEPTH_GEN_INDEX]);
				LOG_DEBUG2("Depth generator supports (%u) user position optimizations", uUserPos);
				for (i=0; i<uUserPos; i++)
				{
					*nRetVal = xnGetUserPosition(x->hProductionNode[DEPTH_GEN_INDEX], i, &box);
					if (*nRetVal)
					{
						LOG_WARNING_RC(*nRetVal, "xnGetUserPositions");
						*nRetVal = XN_STATUS_OK;
					}
					else
					{
						object_post((t_object*)x, "userpos[%u]= (%f, %f, %f) (%f, %f, %f)", i, box.LeftBottomNear.X, box.LeftBottomNear.Y, box.LeftBottomNear.Z, box.RightTopFar.X, box.RightTopFar.Y, box.RightTopFar.Z);
					}
				}
			}
#endif
			break;
		case XN_NODE_TYPE_IMAGE:
			x->hProductionNode[IMAGE_GEN_INDEX] = xnNodeInfoGetRefHandle(pProdNodeInfo);
			xnGetImageMetaData(x->hProductionNode[IMAGE_GEN_INDEX], (XnImageMetaData *)x->pMapMetaData[IMAGEMAP_OUTPUT_INDEX]);
			x->bHaveValidGeneratorProductionNode = true;
			break;
		case XN_NODE_TYPE_IR:
			x->hProductionNode[IR_GEN_INDEX] = xnNodeInfoGetRefHandle(pProdNodeInfo);
			xnGetIRMetaData(x->hProductionNode[IR_GEN_INDEX], (XnIRMetaData *)x->pMapMetaData[IRMAP_OUTPUT_INDEX]);
			x->bHaveValidGeneratorProductionNode = true;
			break;
		case XN_NODE_TYPE_SCENE:
			x->hProductionNode[SCENE_GEN_INDEX] = xnNodeInfoGetRefHandle(pProdNodeInfo);
			// no need to xnGetSceneMetaData()
			x->bHaveValidGeneratorProductionNode = true;
			break;
		case XN_NODE_TYPE_USER:
			x->hProductionNode[USER_GEN_INDEX] = xnNodeInfoGetRefHandle(pProdNodeInfo);
			xnRegisterUserCallbacks(x->hProductionNode[USER_GEN_INDEX], User_NewUser, User_LostUser, x, &(x->hUserCallbacks));
			xnRegisterToUserExit(x->hProductionNode[USER_GEN_INDEX], User_Exit, x, &(x->hUserExitCallback));
			xnRegisterToUserReEnter(x->hProductionNode[USER_GEN_INDEX], User_ReEnter, x, &(x->hUserReEnterCallback));
			xnGetUserPixels(x->hProductionNode[USER_GEN_INDEX], 0, (XnSceneMetaData *)x->pMapMetaData[USERPIXELMAP_OUTPUT_INDEX]);
			x->bHaveValidGeneratorProductionNode = true;

			// check for and then setup skeleton support
			if (xnIsCapabilitySupported(x->hProductionNode[USER_GEN_INDEX], XN_CAPABILITY_SKELETON))
			{
				if (xnIsProfileAvailable(x->hProductionNode[USER_GEN_INDEX], (XnSkeletonProfile)x->siSkeletonProfile))
				{
					if (xnNeedPoseForSkeletonCalibration(x->hProductionNode[USER_GEN_INDEX]))
					{
						if (xnIsCapabilitySupported(x->hProductionNode[USER_GEN_INDEX], XN_CAPABILITY_POSE_DETECTION))
						{
							LOG_DEBUG2("user generator supports %u poses", xnGetNumberOfPoses(x->hProductionNode[USER_GEN_INDEX]));
							xnRegisterToPoseDetected(x->hProductionNode[USER_GEN_INDEX], UserPose_PoseDetected, x, &(x->hPoseCallbacks)); 
							xnGetSkeletonCalibrationPose(x->hProductionNode[USER_GEN_INDEX], x->strRequiredCalibrationPose);
							x->bNeedPose = true;
							x->bHaveSkeletonSupport = true;
						}
						else
						{
							LOG_ERROR("Pose required for skeleton, but user generator doesn't support detecting poses");
						}
					}
					else
					{
						x->bHaveSkeletonSupport = true;
					}
					if (x->bHaveSkeletonSupport)
					{
						xnSetSkeletonProfile(x->hProductionNode[USER_GEN_INDEX], (XnSkeletonProfile)x->siSkeletonProfile);
						// TODO xnEnumerateActiveJoints() and store so later in matrix_calc will only retrieve those joints. Need to them pass the
						// active joint list back to the max wrapper so it only iterates over them.
						xnRegisterToCalibrationStart(x->hProductionNode[USER_GEN_INDEX], UserCalibration_CalibrationStart, x, &(x->hCalibrationStartCallback));
						xnRegisterToCalibrationComplete(x->hProductionNode[USER_GEN_INDEX], UserCalibration_CalibrationComplete, x, &(x->hCalibrationCompleteCallback));
						x->pUserSkeletonJoints = (t_user_and_joints *)sysmem_newptr(sizeof(t_user_and_joints) * MAX_NUM_USERS_SUPPORTED);
						xnSetSkeletonSmoothing(x->hProductionNode[USER_GEN_INDEX],x->fSkeletonSmoothingFactor);
					}
				}
				else
				{
					LOG_ERROR("User generator skeleton capability does not support the skeleton profile requested");
				}
			}
			else
			{
				LOG_DEBUG("User generator doesn't support skeleton");
				// TODO consider user generator output other than a skeleton
			}
			break;
		default:
			LOG_DEBUG2("found unsupported node type", xnProductionNodeTypeToString(xnNodeInfoGetDescription(pProdNodeInfo)->Type));
		}
	}
	xnNodeInfoListFree(pProductionNodeList);
	LOG_DEBUG2("bHaveValidGeneratorProductionNode=%s", (x->bHaveValidGeneratorProductionNode ? "true": "false"));

	*nRetVal = xnStartGeneratingAll(x->pContext);
	CHECK_RC_ERROR_EXIT(*nRetVal, "XMLconfig cannot start all generator nodes");

#ifdef _DEBUG
	if (x->hProductionNode[DEPTH_GEN_INDEX])
	{
		XnMapOutputMode *depthMapModes;
		XnUInt32 i, numDepthMapModes = xnGetSupportedMapOutputModesCount(x->hProductionNode[DEPTH_GEN_INDEX]);
		if (depthMapModes = (XnMapOutputMode *)sysmem_newptr(sizeof(XnMapOutputMode) * numDepthMapModes))
		{
			xnGetSupportedMapOutputModes(x->hProductionNode[DEPTH_GEN_INDEX], depthMapModes, &numDepthMapModes);
			LOG_DEBUG2("== %lu Depth modes avail==", numDepthMapModes);
			for (i=0; i<numDepthMapModes; i++)
			{
				object_post((t_object*)x, "FPS=%lu X=%lu Y=%lu Z=%u", depthMapModes[i].nFPS, depthMapModes[i].nXRes, depthMapModes[i].nYRes, xnGetDeviceMaxDepth(x->hProductionNode[DEPTH_GEN_INDEX]));
			}
			sysmem_freeptr(depthMapModes);
		}

	}

	if (x->hProductionNode[IMAGE_GEN_INDEX])
	{
		XnMapOutputMode *imageMapModes;
		XnUInt32 i, numImageMapModes = xnGetSupportedMapOutputModesCount(x->hProductionNode[IMAGE_GEN_INDEX]);
		if (imageMapModes = (XnMapOutputMode *)sysmem_newptr(sizeof(XnMapOutputMode) * numImageMapModes))
		{
			xnGetSupportedMapOutputModes(x->hProductionNode[IMAGE_GEN_INDEX], imageMapModes, &numImageMapModes);
			LOG_DEBUG2("== %lu Image modes avail==", numImageMapModes);
			for (i=0; i<numImageMapModes; i++)
			{
				object_post((t_object*)x, "FPS=%lu X=%lu Y=%lu", imageMapModes[i].nFPS, imageMapModes[i].nXRes, imageMapModes[i].nYRes);
			}
			sysmem_freeptr(imageMapModes);
		}
	}

	if (x->hProductionNode[USER_GEN_INDEX])
	{
		LOG_DEBUG("== Created user generator ==");
		if (x->bHaveSkeletonSupport)
		{
			LOG_DEBUG("Supports skeletons");
			if (x->bNeedPose)
			{
				LOG_DEBUG("Skeleton needs a pose");
			}
		}
	}
	
	if (x->hProductionNode[IR_GEN_INDEX])
	{
		XnMapOutputMode *IrMapModes;
		XnUInt32 i, numIrMapModes = xnGetSupportedMapOutputModesCount(x->hProductionNode[IR_GEN_INDEX]);
		if (IrMapModes = (XnMapOutputMode *)sysmem_newptr(sizeof(XnMapOutputMode) * numIrMapModes))
		{
			xnGetSupportedMapOutputModes(x->hProductionNode[IR_GEN_INDEX], IrMapModes, &numIrMapModes);
			LOG_DEBUG2("== %lu IR modes avail==", numIrMapModes);
			for (i=0; i<numIrMapModes; i++)
			{
				object_post((t_object*)x, "FPS=%lu X=%lu Y=%lu", IrMapModes[i].nFPS, IrMapModes[i].nXRes, IrMapModes[i].nYRes);
			}
			sysmem_freeptr(IrMapModes);
		}
	}

	object_post((t_object*)x, "==Current active modes==");
	if (x->hProductionNode[DEPTH_GEN_INDEX])
	{
		object_post((t_object*)x, "DepthMD FPS=%lu FullX=%lu FullY=%lu Z=%u", ((XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX])->pMap->nFPS, ((XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX])->pMap->FullRes.X, ((XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX])->pMap->FullRes.Y, ((XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX])->nZRes);
		object_post((t_object*)x, "DepthMD OffsetX=%lu OffsetY=%lu CropX=%lu CropY=%lu", ((XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX])->pMap->Offset.X, ((XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX])->pMap->Offset.Y, ((XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX])->pMap->Res.X, ((XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX])->pMap->Res.Y);
		object_post((t_object*)x, "DepthMD PixelFormat=%s", xnPixelFormatToString(((XnDepthMetaData *)x->pMapMetaData[DEPTHMAP_OUTPUT_INDEX])->pMap->PixelFormat));
	}
	if (x->hProductionNode[IMAGE_GEN_INDEX])
	{
		object_post((t_object*)x, "ImageMD FPS=%lu X=%lu Y=%lu", ((XnImageMetaData *)x->pMapMetaData[IMAGEMAP_OUTPUT_INDEX])->pMap->nFPS, ((XnImageMetaData *)x->pMapMetaData[IMAGEMAP_OUTPUT_INDEX])->pMap->FullRes.X, ((XnImageMetaData *)x->pMapMetaData[IMAGEMAP_OUTPUT_INDEX])->pMap->FullRes.Y);
		object_post((t_object*)x, "ImageMD PixelFormat=%s", xnPixelFormatToString(((XnImageMetaData *)x->pMapMetaData[IMAGEMAP_OUTPUT_INDEX])->pMap->PixelFormat));
	}
	if (x->hProductionNode[USER_GEN_INDEX])
	{
		object_post((t_object*)x, "UserPixelMD FPS=%lu X=%lu Y=%lu", ((XnSceneMetaData *)x->pMapMetaData[USERPIXELMAP_OUTPUT_INDEX])->pMap->nFPS, ((XnSceneMetaData *)x->pMapMetaData[USERPIXELMAP_OUTPUT_INDEX])->pMap->FullRes.X, ((XnSceneMetaData *)x->pMapMetaData[USERPIXELMAP_OUTPUT_INDEX])->pMap->FullRes.Y);
		object_post((t_object*)x, "UserPixelMD PixelFormat=%s", xnPixelFormatToString(((XnSceneMetaData *)x->pMapMetaData[USERPIXELMAP_OUTPUT_INDEX])->pMap->PixelFormat));
	}
	if (x->hProductionNode[IR_GEN_INDEX])
	{
		object_post((t_object*)x, "IrMD FPS=%lu X=%lu Y=%lu", ((XnIRMetaData *)x->pMapMetaData[IRMAP_OUTPUT_INDEX])->pMap->nFPS, ((XnIRMetaData *)x->pMapMetaData[IRMAP_OUTPUT_INDEX])->pMap->FullRes.X, ((XnIRMetaData *)x->pMapMetaData[IRMAP_OUTPUT_INDEX])->pMap->FullRes.Y);
		object_post((t_object*)x, "IrMD PixelFormat=%s", xnPixelFormatToString(((XnIRMetaData *)x->pMapMetaData[IRMAP_OUTPUT_INDEX])->pMap->PixelFormat));
	}
#endif

}
// ---------- user and skeleton generator code -----------------

// register to receive callbacks for jit_open object events
t_jit_err RegisterJitOpenNIEventCallbacks(t_jit_openni *x, JitOpenNIEventHandler funcCallback, void **hUnregister)
{
	// TODO make a critical region critical_enter() for navigating the linked list in register and unregister
	LOG_DEBUG2("starting size of linklist=%ld", jit_linklist_getsize(x->pEventCallbackFunctions));
	if ( jit_linklist_append(x->pEventCallbackFunctions, funcCallback) == -1)		// TODO, I want to use jit_linklist_insertindex() here but it crashes on an empty list
	{
		return JIT_ERR_OUT_OF_MEM;
	}
	else
	{
		*hUnregister = jit_linklist_index2ptr(x->pEventCallbackFunctions, jit_linklist_objptr2index(x->pEventCallbackFunctions, funcCallback));	// this is not thread safe, however, Max APIs are limiting me
		LOG_DEBUG3("added regid=%x, ending size of linklist=%ld", *hUnregister, jit_linklist_getsize(x->pEventCallbackFunctions));
	}
	return JIT_ERR_NONE;
}

t_jit_err UnregisterJitOpenNIEventCallbacks(t_jit_openni *x, void *pUnregister)
{
	LOG_DEBUG2("unregister: starting size of linklist=%ld", jit_linklist_getsize(x->pEventCallbackFunctions));
	LOG_DEBUG2("unregister: About to unregister using RegID=%x", pUnregister);
	if (jit_linklist_chuckptr(x->pEventCallbackFunctions, pUnregister) == -1)
	{
		LOG_DEBUG("linklist_delete returned an error");
		LOG_DEBUG2("unregister: ending size of linklist=%ld", jit_linklist_getsize(x->pEventCallbackFunctions));
		return JIT_ERR_OUT_OF_BOUNDS;
	}
	else
	{
		LOG_DEBUG2("unregister: ending size of linklist=%ld", jit_linklist_getsize(x->pEventCallbackFunctions));
		return JIT_ERR_NONE;
	}
}

void makeCallbacks(t_jit_openni *x, enum JitOpenNIEvents iEventType, XnUserID userID)
{
	long i;
	JitOpenNIEventHandler funcCallback;

	for (i=0; i < jit_linklist_getsize(x->pEventCallbackFunctions); i++)	// I call for the size each loop to help catch resizing of list by another thread
	{
		if (funcCallback = (JitOpenNIEventHandler)jit_linklist_getindex(x->pEventCallbackFunctions, i))
		{
			funcCallback(x, iEventType, userID);
		}
	}

}

// Callback: New user was detected
void XN_CALLBACK_TYPE User_NewUser(XnNodeHandle hUserGenerator, XnUserID userID, t_jit_openni *x)
{
	if (x->bHaveSkeletonSupport)
	{
		if (x->bNeedPose)		// for now am restricting pose detection to only be the pose for skeleton detection
		{
			xnStartPoseDetection(hUserGenerator, x->strRequiredCalibrationPose, userID);
		}
		else
		{
			xnRequestSkeletonCalibration(hUserGenerator, userID, true);
		}
	}
	makeCallbacks(x, JITOPENNI_NEW_USER, userID);
}

// Callback: An existing user was lost
void XN_CALLBACK_TYPE User_LostUser(XnNodeHandle hUserGenerator, XnUserID userID, t_jit_openni *x)
{
	if (x->bNeedPose) xnStopPoseDetection(hUserGenerator, userID);
	makeCallbacks(x, JITOPENNI_LOST_USER, userID);
}

// Callback: An existing user exited the scene (but not lost yet)
void XN_CALLBACK_TYPE User_Exit(XnNodeHandle hUserGenerator, XnUserID userID, t_jit_openni *x)
{
	makeCallbacks(x, JITOPENNI_EXIT_USER, userID);
}

// Callback: An existing user re-entered the scene after exiting
void XN_CALLBACK_TYPE User_ReEnter(XnNodeHandle hUserGenerator, XnUserID userID, t_jit_openni *x)
{
	makeCallbacks(x, JITOPENNI_REENTER_USER, userID);
}

// Callback: Detected a pose
void XN_CALLBACK_TYPE UserPose_PoseDetected(XnNodeHandle hPoseCapability, const XnChar *strPose, XnUserID userID, t_jit_openni *x)
{
	xnStopPoseDetection(hPoseCapability, userID);
	if (strcmp(x->strRequiredCalibrationPose, strPose) == 0)
	{
		xnRequestSkeletonCalibration(hPoseCapability, userID, true);
	}
	makeCallbacks(x, JITOPENNI_CALIB_POSE_DETECTED, userID);
}

// Callback: Started calibration
void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(XnNodeHandle hSkeletonCapability, XnUserID userID, t_jit_openni *x)
{
	makeCallbacks(x, JITOPENNI_CALIB_START, userID);
}

// Callback: Finished calibration
void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(XnNodeHandle hSkeletonCapability, XnUserID userID, XnCalibrationStatus calibrationResult, t_jit_openni *x)
{
	if (calibrationResult == XN_CALIBRATION_STATUS_OK)
	{
		// Calibration succeeded
		xnStartSkeletonTracking(hSkeletonCapability, userID);
		makeCallbacks(x, JITOPENNI_CALIB_SUCCESS, userID);
	}
	else
	{
		// Calibration failed
		if (x->bHaveSkeletonSupport)
		{
			if (x->bNeedPose)		// for now am restricting pose detection to only be the pose for skeleton detection
			{
				xnStartPoseDetection(hSkeletonCapability, x->strRequiredCalibrationPose, userID);
			}
			else
			{
				xnRequestSkeletonCalibration(hSkeletonCapability, userID, true);
			}
		}
		makeCallbacks(x, JITOPENNI_CALIB_FAIL, userID);
	}
}

t_jit_err jit_openni_depthfov_get(t_jit_openni *x, void *attr, long *ac, t_atom **av)
{
	XnFieldOfView xFieldOfView;

	if ((*ac)&&(*av))
	{
		//memory passed in, use it
	}
	else
	{
		//otherwise allocate memory
		*ac = 2;
		if (!(*av = (t_atom *)jit_getbytes(sizeof(t_atom)*(*ac))))
		{
			*ac = 0;
			return JIT_ERR_OUT_OF_MEM;
		}
	}
	if (x->hProductionNode[DEPTH_GEN_INDEX])
	{
		xnGetDepthFieldOfView(x->hProductionNode[DEPTH_GEN_INDEX], &xFieldOfView);
		jit_atom_setfloat(*av, xFieldOfView.fHFOV);
		jit_atom_setfloat((*av) + 1, xFieldOfView.fVFOV);
	}
	return JIT_ERR_NONE;
}

t_jit_err jit_openni_scenefloor_get(t_jit_openni *x, void *attr, long *ac, t_atom **av)
{
	XnPlane3D planeFloor;

	if ((*ac)&&(*av))
	{
		//memory passed in, use it
	}
	else
	{
		//otherwise allocate memory
		*ac = 6;
		if (!(*av = (t_atom *)jit_getbytes(sizeof(t_atom)*(*ac))))
		{
			*ac = 0;
			return JIT_ERR_OUT_OF_MEM;
		}
	}

	if (x->hProductionNode[SCENE_GEN_INDEX])
	{
		// no need to xnGetSceneMetaData()
		xnGetFloor(x->hProductionNode[SCENE_GEN_INDEX], &planeFloor);
		switch (x->siSkeletonValueType) // case 0 is the default native OpenNI values (real world)
		{
		case 1:		// the OpenNI projective coordinates
			xnConvertRealWorldToProjective(x->hProductionNode[DEPTH_GEN_INDEX], 1, &(planeFloor.ptPoint), &(planeFloor.ptPoint));
			break;
		case 2:		// legacy "normalized" OSCeleton values
			// I do not support the legacy OSCeleton multiplier and offset, if a dev is that advanced, they can do the simple conversion to this object's native output
			// also, the legacy OSCeleton "normalization" formula used here is the same as for OSCeleton joints. Note that OSCeleton has a legacy bug in that
			// the "normalization" formulas for center of mass and joints differ. Since OSCeleton doesn't support floor data at all, I can only guess that a user
			// might want to know when a foot joint is near the floor and therefore use the legacy OSCeleton joint normalization formula
			planeFloor.ptPoint.X = (1280 - planeFloor.ptPoint.X) / 2560;	// "Normalize" coords to 0..1 interval
			planeFloor.ptPoint.Y = (960 - planeFloor.ptPoint.Y) / 1920;	// "Normalize" coords to 0..1 interval
			planeFloor.ptPoint.Z = planeFloor.ptPoint.Z * 7.8125 / 10000;	// "Normalize" coords to 0..7.8125 interval
			break;
		}
		jit_atom_setfloat(*av, planeFloor.ptPoint.X);
		jit_atom_setfloat((*av) + 1, planeFloor.ptPoint.Y);
		jit_atom_setfloat((*av) + 2, planeFloor.ptPoint.Z);
		jit_atom_setfloat((*av) + 3, planeFloor.vNormal.X);
		jit_atom_setfloat((*av) + 4, planeFloor.vNormal.Y);
		jit_atom_setfloat((*av) + 5, planeFloor.vNormal.Z);
		return JIT_ERR_NONE;
	}
	else
	{
		return JIT_ERR_DATA_UNAVAILABLE;
	}
}