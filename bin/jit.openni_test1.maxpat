{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 652.0, 96.0, 903.0, 861.0 ],
		"bglocked" : 0,
		"defrect" : [ 652.0, 96.0, 903.0, 861.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "jit.fpsgui",
					"id" : "obj-15",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 510.0, 300.0, 80.0, 36.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"fontname" : "Arial",
					"mode" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.fpsgui",
					"id" : "obj-9",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 15.0, 315.0, 80.0, 36.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"fontname" : "Arial",
					"mode" : 3
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"id" : "obj-7",
					"presentation_rect" : [ 0.0, 0.0, 320.0, 240.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 525.0, 450.0, 320.0, 240.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"id" : "obj-2",
					"presentation_rect" : [ 0.0, 0.0, 320.0, 240.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 165.0, 450.0, 320.0, 240.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "getdimstride",
					"id" : "obj-16",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 90.0, 240.0, 76.0, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.matrix 1 char 13 7",
					"id" : "obj-12",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 405.0, 360.0, 119.0, 20.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "jit_matrix", "" ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "getcell 320 240",
					"id" : "obj-17",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 255.0, 285.0, 92.0, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "getdim",
					"id" : "obj-13",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 195.0, 285.0, 47.0, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"id" : "obj-11",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 225.0, 420.0, 34.0, 20.0 ],
					"numoutlets" : 0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.matrix",
					"id" : "obj-10",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 120.0, 330.0, 55.0, 20.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "jit_matrix", "" ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"id" : "obj-14",
					"numinlets" : 1,
					"patching_rect" : [ 120.0, 60.0, 20.0, 20.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "metro 500",
					"id" : "obj-8",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 120.0, 105.0, 65.0, 20.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"id" : "obj-5",
					"numinlets" : 1,
					"patching_rect" : [ 60.0, 120.0, 20.0, 20.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "read jit.openni_config.xml",
					"id" : "obj-6",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 300.0, 120.0, 147.0, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "read",
					"id" : "obj-3",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 240.0, 120.0, 35.0, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "read dale 1 2.3",
					"id" : "obj-4",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 465.0, 120.0, 91.0, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.openni",
					"id" : "obj-1",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 240.0, 195.0, 59.0, 20.0 ],
					"numoutlets" : 3,
					"outlettype" : [ "jit_matrix", "jit_matrix", "" ],
					"fontname" : "Arial"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 1 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 1 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 1 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 1 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
