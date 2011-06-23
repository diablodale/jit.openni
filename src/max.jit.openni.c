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
static void	*max_jit_openni_class = NULL;


/************************************************************************************/

int main(void)
{	
	void *p, *q;
	
	post("jit.openni %s, Copyright (c) 2011 Dale Phurrough. This program comes with ABSOLUTELY NO WARRANTY.", JIT_OPENNI_VERSION);
	post("jit.openni %s, Licensed under the GNU General Public License v3.0 (GPLv3) available at http://www.gnu.org/licenses/gpl-3.0.html", JIT_OPENNI_VERSION);

	// initialize the Jitter class by calling Jitter class's registration function
	jit_openni_init();
	
	// create the Max wrapper class
	setup((t_messlist**)&max_jit_openni_class, (method)max_jit_openni_new, (method)max_jit_openni_free, sizeof(t_max_jit_openni), 0, A_GIMME, 0);
	
	// specify a byte offset to keep additional OBEX information
	p = max_jit_classex_setup(calcoffset(t_max_jit_openni, obex));
	
	// look up the Jitter class in the class registry
	q = jit_class_findbyname(gensym("jit_openni"));    
    
	// add default methods and attributes for MOP max wrapper class
	max_jit_classex_mop_wrap(p, q, MAX_JIT_MOP_FLAGS_OWN_OUTPUTMATRIX|MAX_JIT_MOP_FLAGS_OWN_JIT_MATRIX|MAX_JIT_MOP_FLAGS_OWN_ADAPT); // attrs & methods for name, type, dim, planecount, bang, outputmatrix, etc

	// wrap the Jitter class with the standard methods for Jitter objects, e.g. getattributes, dumpout, maxjitclassaddmethods, etc
	max_jit_classex_standard_wrap(p, q, 0);

    // add methods to the Max wrapper class
	max_addmethod_usurp_low((method)max_jit_openni_outputmatrix, "outputmatrix");	
	max_addmethod_usurp_low((method)max_jit_openni_XMLConfig_read, "read");

	// add an inlet/outlet assistance method; in this case the default matrix-operator (mop) assist fn 
	addmess((method)max_jit_mop_assist, "assist", A_CANT, 0);
	return 0;
}


/************************************************************************************/
// Object Life Cycle

void *max_jit_openni_new(t_symbol *s, long argc, t_atom *argv)
{
	t_max_jit_openni	*x;
	void				*o;
	long				i;
	
	x = (t_max_jit_openni*)max_jit_obex_new(max_jit_openni_class, gensym("jit_openni"));
	if (x)
	{
		o = jit_object_new(gensym("jit_openni"));	// instantiate jit.openni jitter object
		if (o)
		{
			max_jit_mop_setup_simple(x, o, argc, argv);	// handle standard MOP max wrapper setup tasks
			max_jit_attr_args(x, argc, argv); // process attribute arguments, like auto handling of @attribute's
#ifdef _DEBUG
			for (i = 0; i < argc; i++)
			{
				switch (atom_gettype(&(argv[i])))
				{
					case A_LONG:
						LOG_COMMENT3("arg %ld: long (%ld)", i, atom_getlong(&(argv[i])));
						break;
					case A_FLOAT:
						LOG_COMMENT3("arg %ld: float (%f)", i, atom_getfloat(&(argv[i])));
						break;
					case A_SYM:
						LOG_COMMENT3("arg %ld: symbol (%s)", i, atom_getsym(&(argv[i]))->s_name);
						break;
					default:
						LOG_WARNING2("arg %ld: forbidden argument", i); 
				}
			}
#endif
		} 
		else
		{
			LOG_ERROR("jit.openni: could not allocate object");
			freeobject((t_object*)x);
			x = NULL;
		}
	}
	return(x);
}


void max_jit_openni_free(t_max_jit_openni *x)
{
	max_jit_mop_free(x);
	
	// lookup the internal Jitter object instance and free
	jit_object_free(max_jit_obex_jitob_get(x));
	
	// free resources associated with the obex entry
	max_jit_obex_free(x);
}

void max_jit_openni_XMLConfig_read(t_max_jit_openni *x, t_symbol *s, short argc, t_atom *argv)
{
	long i;
	t_atom *ap;	
	short filePathID;
	long fileType = 'TEXT', outType;
	char filename[MAX_FILENAME_CHARS];
	char fullyQualifiedPathname[MAX_PATH_CHARS];
	
#ifdef _DEBUG
	t_object *mypatcher;
	t_symbol *mypatcherpath;

	if (object_obex_lookup(x, gensym("#P"), &mypatcher) != MAX_ERR_NONE)
		LOG_ERROR("error getting patcher for jit.openni");
	LOG_DEBUG2("my patcher is at address %lx",mypatcher);
	mypatcherpath = object_attr_getsym(mypatcher, gensym("filepath"));
	
	// BUGBUG if I use _sym_nothing rather than gensym("") then I get linker error LNK2001: unresolved external symbol __common_symbols
	if ((mypatcherpath) && (mypatcherpath != gensym("")))
	{
		LOG_COMMENT2("The patcher path is %s", mypatcherpath->s_name);
	}
	else
	{
		LOG_COMMENT("error getting filepath symbol for max.jit.openni");
		return;
	}
#endif

	if (argc == 0) // if no argument supplied, ask for file
	{
		if (open_dialog(filename, &filePathID, &outType, &fileType, 1)) //BUGBUG
		{
			// non-zero: user cancelled or error
			LOG_ERROR("error getting XML config file from dialog box for max.jit.openni");
			return;
		}
	}
	else if ((argc != 1) || (atom_gettype(argv) != A_SYM))
	{
		LOG_ERROR("read must have only one symbol argument");
		return;
	}
	else // we have exactly one symbol argument
	{
		strncpy_zero(filename, atom_getsym(argv)->s_name, MAX_FILENAME_CHARS);
		if (locatefile_extended(filename, &filePathID, &outType, &fileType, 1))
		{
			LOG_ERROR2("Could not find file", atom_getsym(argv)->s_name);
			return;
		}
	}

	//Load file
	if (path_topathname(filePathID, filename, fullyQualifiedPathname) == 0)
	{
		LOG_DEBUG2("asking Jitter object to load file %s", fullyQualifiedPathname);
		jit_object_method(max_jit_obex_jitob_get(x), gensym("init_from_xml"), gensym(fullyQualifiedPathname));
	}

}

void max_jit_openni_outputmatrix(t_max_jit_openni *x)
{
	t_atom a;
	long outputmode = max_jit_mop_getoutputmode(x);
	void *mop = max_jit_obex_adornment_get(x,_jit_sym_jit_mop);
	t_jit_err err;	
	
	LOG_DEBUG("starting custom outputmatrix");
	if (outputmode && mop)
	{ //always output unless output mode is none
		if (outputmode==1)
		{
			LOG_DEBUG("about to call matrix_calc from custom outputmatrix");
			if (err=(t_jit_err)jit_object_method(max_jit_obex_jitob_get(x), _jit_sym_matrix_calc,
				jit_object_method(mop,_jit_sym_getinputlist),
				jit_object_method(mop,_jit_sym_getoutputlist)))						
			{
				jit_error_code(x,err); 
			}
			else
			{
				LOG_DEBUG("called matrix_calc(), now calling outputmatrix()");
				max_jit_mop_outputmatrix(x);
				LOG_DEBUG("called outputmatrix()");
			}
		}
		else if (outputmode==3)
		{
			LOG_DEBUG("bypassing matrix_calc(), now calling outputmatrix()");
			max_jit_mop_outputmatrix(x);
			LOG_DEBUG("called outputmatrix()");
		}
	}	
}