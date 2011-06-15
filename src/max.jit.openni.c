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

#include "targetver.h"
#include "ext.h"
#include "ext_obex.h"
#include "jit.common.h"
#include "max.jit.mop.h"


// Max object instance data
// Note: most instance data is in the Jitter object which we will wrap
typedef struct _max_jit_openni {
	t_object	ob;
	void		*obex;
} t_max_jit_openni;


// prototypes
//BEGIN_USING_C_LINKAGE
t_jit_err	jit_openni_init(void);
void		*max_jit_openni_new(t_symbol *s, long argc, t_atom *argv);
void		max_jit_openni_free(t_max_jit_openni *x);
void		max_jit_openni_XMLConfig_read(t_max_jit_openni *x, t_symbol *s, short argc, t_atom *argv);
//END_USING_C_LINKAGE

// globals
static void	*max_jit_openni_class = NULL;


/************************************************************************************/

int main(void)
{	
	void *p, *q;
	
	jit_openni_init();	
	setup((t_messlist**)&max_jit_openni_class, (method)max_jit_openni_new, (method)max_jit_openni_free, sizeof(t_max_jit_openni), 0, A_GIMME, 0);

	p = max_jit_classex_setup(calcoffset(t_max_jit_openni, obex));
	q = jit_class_findbyname(gensym("jit_openni"));    
    max_jit_classex_mop_wrap(p, q, MAX_JIT_MOP_FLAGS_OWN_BANG|MAX_JIT_MOP_FLAGS_OWN_OUTPUTMATRIX|MAX_JIT_MOP_FLAGS_OWN_JIT_MATRIX); // attrs & methods for name, type, dim, planecount, bang, outputmatrix, etc
    max_jit_classex_standard_wrap(p, q, 0);						// attrs & methods for getattributes, dumpout, maxjitclassaddmethods, etc
    max_addmethod_usurp_low((method)max_jit_openni_XMLConfig_read, "read");
	addmess((method)max_jit_mop_assist, "assist", A_CANT, 0);	// standard matrix-operator (mop) assist fn 
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
		o = jit_object_new(gensym("jit_openni"));
		if (o)
		{
			max_jit_mop_setup_simple(x, o, argc, argv);			
			max_jit_attr_args(x, argc, argv);
			for (i = 0; i < argc; i++)
			{
				switch (atom_gettype(&(argv[i])))
				{
					case A_LONG:
						object_post((t_object *)x, "arg %ld: long (%ld)", i, atom_getlong(&(argv[i])));
						break;
					case A_FLOAT:
						object_post((t_object *)x, "arg %ld: float (%f)", i, atom_getfloat(&(argv[i])));
						break;
					case A_SYM:
						object_post((t_object *)x, "arg %ld: symbol (%s)", i, atom_getsym(&(argv[i]))->s_name);
						break;
					default:
						object_error((t_object *)x, "arg %ld: forbidden argument", i); 
				}
			}
		} 
		else
		{
			jit_object_error((t_object*)x, "jit.openni: could not allocate object");
			freeobject((t_object*)x);
			x = NULL;
		}
	}
	return(x);
}


void max_jit_openni_free(t_max_jit_openni *x)
{
	max_jit_mop_free(x);
	jit_object_free(max_jit_obex_jitob_get(x));
	max_jit_obex_free(x);
}

void max_jit_openni_XMLConfig_read(t_max_jit_openni *x, t_symbol *s, short argc, t_atom *argv)
{
	long i;
	t_atom *ap;	
	t_object *mypatcher;
	t_symbol *mypatcherpath;
	short filePathID;
	long fileType = 'TEXT', outType;
	char filename[MAX_FILENAME_CHARS];
	char fullyQualifiedPathname[MAX_PATH_CHARS];
	
	if (object_obex_lookup(x, gensym("#P"), &mypatcher) != MAX_ERR_NONE)
		object_error((t_object*)x, "error getting patcher for jit.openni");
	object_post((t_object*)x, "my patcher is at address %lx",mypatcher);
	mypatcherpath = object_attr_getsym(mypatcher, gensym("filepath"));
	
	// BUGBUG if I use _sym_nothing rather than gensym("") then I get linker error LNK2001: unresolved external symbol __common_symbols
	if ((mypatcherpath) && (mypatcherpath != gensym("")))
	{
		object_post((t_object*)x, "The patcher path is %s", mypatcherpath->s_name);
	}
	else
	{
		object_error((t_object*)x, "error getting filepath symbol for max.jit.openni");
		return;
	}

	if (argc == 0) // if no argument supplied, ask for file
	{
		if (open_dialog(filename, &filePathID, &outType, &fileType, 1)) //BUGBUG
		{
			// non-zero: user cancelled or error
			object_error((t_object*)x, "error getting XML config file from dialog box for max.jit.openni");
			return;
		}
	}
	else if ((argc != 1) || (atom_gettype(argv) != A_SYM))
	{
		object_error((t_object*)x, "read must have only one symbol argument");
		return;
	}
	else // we have exactly one symbol argument
	{
		strncpy_zero(filename, atom_getsym(argv)->s_name, MAX_FILENAME_CHARS);
		if (locatefile_extended(filename, &filePathID, &outType, &fileType, 1))
		{
			error("Could not find file %s",atom_getsym(argv)->s_name);
			return;
		}
	}

	//Load file
	if (path_topathname(filePathID, filename, fullyQualifiedPathname) == 0)
	{
		object_post((t_object*)x, "now able to load file %s", fullyQualifiedPathname);
		//jit_openni_init_from_xml(max_jit_obex_jitob_get(x), gensym(fullyQualifiedPathname));
		jit_object_method(max_jit_obex_jitob_get(x), gensym("init_from_xml"), gensym(fullyQualifiedPathname));
	}

}
