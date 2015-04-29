/*-------------------------------------------------------------*/
/*               YACC or BISON Specification                   */
/*-------------------------------------------------------------*/

%locations
/*%token_table*/

%{
/************* FIRST USER DECLARATIONS *****/

#include <stdio.h>
#include <string.h>
#include "grammar.h"
#include "globals.h"



#define YYDEBUG 1


#ifndef yyerror
extern char *yytext;
#define yyerror(x) { \
        syntaxerror(yylloc.first_line, yylloc.first_column, \
                 "unexpected %s \"%s\" (%s)", \
                 (yychar<0)?"(?)":yytname[YYTRANSLATE(yychar)], \
                 strlen(yytext)<48?yytext:"(?)", \
                 x); \
    }
#endif


#ifndef yylocate
#define yylocate(x) (&(x))
#endif


%}


/*--------------------------------------------------------------------*/
/* Tokens from scanner                                                */
/*--------------------------------------------------------------------*/

%token LEXWORD_ABOVE
%token LEXWORD_ANCHORPOINTS
%token LEXWORD_ANCHOR
%token LEXWORD_AQUAMARINE
%token LEXWORD_AROUND
%token LEXWORD_ARROWMODE
%token LEXWORD_ARROWHEIGHT
%token LEXWORD_ARROWWIDTH
%token LEXWORD_ARROWCOLOR
%token LEXWORD_ARROWSTYLE
%token LEXWORD_ARROWSIZE
%token LEXWORD_BARROWCOLOR
%token LEXWORD_BARROWSTYLE
%token LEXWORD_BARROWSIZE
%token LEXWORD_BACKEDGE
%token LEXWORD_BARYCENTER
%token LEXWORD_BARY
%token LEXWORD_BARYMEDIAN
%token LEXWORD_BEHIND
%token LEXWORD_BELOW
%token LEXWORD_BLACK
%token LEXWORD_BLUE
%token LEXWORD_BMAX
%token LEXWORD_BORDERCOLOR
%token LEXWORD_BORDERWIDTH
%token LEXWORD_BOTTOM_MARGIN
%token LEXWORD_BOTTOM_TO_TOP
%token LEXWORD_BOTTOM
%token LEXWORD_BOX
%token LEXWORD_BENTNEAREDGE
%token LEXWORD_CENTER
%token LEXWORD_CFISH
%token LEXWORD_CLASSNAME
%token LEXWORD_CLASS
%token LEXWORD_CLUSTER
%token LEXWORD_CMIN
%token LEXWORD_CMAX
%token LEXWORD_COLORENTRY
%token LEXWORD_COLOR
%token LEXWORD_CONSTRAINTS
%token LEXWORD_CONSTRAINT
%token LEXWORD_CONTINUOUS
%token LEXWORD_CROSSING_WEIGHT
%token LEXWORD_CROSSING_OPT
%token LEXWORD_CROSSING_P2
%token LEXWORD_CYAN
%token LEXWORD_DARKBLUE
%token LEXWORD_DARKCYAN
%token LEXWORD_DARKGREEN
%token LEXWORD_DARKGREY
%token LEXWORD_DARKMAGENTA
%token LEXWORD_DARKRED
%token LEXWORD_DARKYELLOW
%token LEXWORD_DASHED
%token LEXWORD_DFS
%token LEXWORD_DIMENSION
%token LEXWORD_DIRTY_EDGE_LABELS
%token LEXWORD_DISPLAY_EDGE_LABELS
%token LEXWORD_DOTTED
%token LEXWORD_EDGE1
%token LEXWORD_EDGE2
%token LEXWORD_EDGES
%token LEXWORD_ELLIPSE
%token LEXWORD_EQUAL_COLUMN
%token LEXWORD_EQUAL_POSITION
%token LEXWORD_EQUAL_ROW
%token LEXWORD_EQUAL
%token LEXWORD_EVERY
%token LEXWORD_FCFISH
%token LEXWORD_FPFISH
%token LEXWORD_FIXED
%token LEXWORD_FREE
%token LEXWORD_FINETUNING
%token LEXWORD_FOLDEDGE
%token LEXWORD_FOLDNODE
%token LEXWORD_FOLDING
%token LEXWORD_FONTNAME
%token LEXWORD_GOLD
%token LEXWORD_GRAPH
%token LEXWORD_GREATER
%token LEXWORD_GREEN
%token LEXWORD_GREY
%token LEXWORD_HEIGHT
%token LEXWORD_HIDESINGLES
%token LEXWORD_HIGH_MARGIN
%token LEXWORD_HIGH
%token LEXWORD_HIDDEN
%token LEXWORD_HORDER
%token LEXWORD_ICONFILE
%token LEXWORD_ICONHEIGHT
%token LEXWORD_ICONSTYLE
%token LEXWORD_ICONWIDTH
%token LEXWORD_INCLUDE
%token LEXWORD_INFONAME
%token LEXWORD_INFO1
%token LEXWORD_INFO2
%token LEXWORD_INFO3
%token LEXWORD_INPUTFUNCTION
%token LEXWORD_INTERVAL
%token LEXWORD_INVISIBLE
%token LEXWORD_IN_FRONT
%token LEXWORD_ISI
%token LEXWORD_KHAKI
%token LEXWORD_TEXTCOLOR
%token LEXWORD_LABEL
%token LEXWORD_LATE_LABELS
%token LEXWORD_LAYOUTALGORITHM
%token LEXWORD_LAYOUTFREQUENCY
%token LEXWORD_LAYOUTPARAMETER
%token LEXWORD_LAYOUTDOWNFACTOR
%token LEXWORD_LAYOUTUPFACTOR
%token LEXWORD_LAYOUTNEARFACTOR
%token LEXWORD_LAYOUTSPLINEFACTOR
%token LEXWORD_LEFT_JUSTIFY
%token LEXWORD_LEFT_MARGIN
%token LEXWORD_LEFT_NEIGHBOR
%token LEXWORD_LEFT_TO_RIGHT
%token LEXWORD_LEFT
%token LEXWORD_LEVEL
%token LEXWORD_VORDER
%token LEXWORD_LIGHTBLUE
%token LEXWORD_LIGHTCYAN
%token LEXWORD_LIGHTGREEN
%token LEXWORD_LIGHTGREY
%token LEXWORD_LIGHTMAGENTA
%token LEXWORD_LIGHTRED
%token LEXWORD_LIGHTYELLOW
%token LEXWORD_LILAC
%token LEXWORD_LIMIT
%token LEXWORD_LINE
%token LEXWORD_LINESTYLE
%token LEXWORD_LOC
%token LEXWORD_LOWER_NEIGHBOR
%token LEXWORD_LOW_MARGIN
%token LEXWORD_LOW
%token LEXWORD_MAGENTA
%token LEXWORD_MANHATTEN
%token LEXWORD_MANUAL
%token LEXWORD_MAXDEPTHSLOW
%token LEXWORD_MAXDEPTH
%token LEXWORD_MAXDEGREE
%token LEXWORD_MAXINDEGREE
%token LEXWORD_MAXOUTDEGREE
%token LEXWORD_MEDIAN
%token LEXWORD_MEDIANBARY
%token LEXWORD_MINDEPTHSLOW
%token LEXWORD_MINDEPTH
%token LEXWORD_MINDEGREE
%token LEXWORD_MININDEGREE
%token LEXWORD_MINOUTDEGREE
%token LEXWORD_MINBACK
%token LEXWORD_NAME
%token LEXWORD_NEAREDGE
%token LEXWORD_NEIGHBORS
%token LEXWORD_NEAREDGES
%token LEXWORD_NONEAREDGES
%token LEXWORD_NODE1
%token LEXWORD_NODE2
%token LEXWORD_NODES
%token LEXWORD_NODE_ALIGN
%token LEXWORD_NONE
%token LEXWORD_NO
%token LEXWORD_ORANGE
%token LEXWORD_ORCHID
%token LEXWORD_ORIENTATION
%token LEXWORD_OUTPUTFUNCTION
%token LEXWORD_PFISH
%token LEXWORD_PINK
%token LEXWORD_PLANAR
%token LEXWORD_PMIN
%token LEXWORD_PMAX
%token LEXWORD_PORTSHARING
%token LEXWORD_PRIORITYPHASE
%token LEXWORD_PRIORITY
%token LEXWORD_PURPLE
%token LEXWORD_RANGE
%token LEXWORD_RED
%token LEXWORD_RHOMB
%token LEXWORD_RIGHT_JUSTIFY
%token LEXWORD_RIGHT_MARGIN
%token LEXWORD_RIGHT_NEIGHBOR
%token LEXWORD_RIGHT_TO_LEFT
%token LEXWORD_RIGHT
%token LEXWORD_RMIN
%token LEXWORD_RMAX
%token LEXWORD_SCALING
%token LEXWORD_SHAPE
%token LEXWORD_SHRINK
%token LEXWORD_SMAX
%token LEXWORD_SMANHATTEN
%token LEXWORD_SIZE
%token LEXWORD_SMALLER
%token LEXWORD_SOLID
%token LEXWORD_SOURCENAME
%token LEXWORD_SPLINES
%token LEXWORD_SPREADLEVEL
%token LEXWORD_STATUS
%token LEXWORD_STRETCH
%token LEXWORD_STRAIGHTPHASE
%token LEXWORD_TARGETNAME
%token LEXWORD_TEXTMODE
%token LEXWORD_THICKNESS
%token LEXWORD_TITLE
%token LEXWORD_TOPSORT
%token LEXWORD_TOP_MARGIN
%token LEXWORD_TOP_TO_BOTTOM
%token LEXWORD_TOP
%token LEXWORD_TREE
%token LEXWORD_TREEFACTOR
%token LEXWORD_TRIANGLE
%token LEXWORD_TURQUOISE
%token LEXWORD_TYPENAME
%token LEXWORD_UPPER_NEIGHBOR
%token LEXWORD_VIEW
%token LEXWORD_WHITE
%token LEXWORD_WIDTH
%token LEXWORD_XBASE
%token LEXWORD_XMAX
%token LEXWORD_XRASTER
%token LEXWORD_XLRASTER
%token LEXWORD_XSCROLLBAR
%token LEXWORD_XSPACE
%token LEXWORD_XLSPACE
%token LEXWORD_YBASE
%token LEXWORD_YELLOWGREEN
%token LEXWORD_YELLOW
%token LEXWORD_YES
%token LEXWORD_YMAX
%token LEXWORD_YRASTER
%token LEXWORD_YSCROLLBAR
%token LEXWORD_YSPACE
%token LEX_INT
%token LEX_FLOAT
%token LEX_CHAR
%token LEX_STRING


/*-------------------------------------------------------------*/
/*                  YYSTYPE definition                         */
/*-------------------------------------------------------------*/

%union {
	unsigned char      byte;
	short int          snum;
	unsigned short int usnum;
	int                num;
	unsigned int       unum;
	long int           lnum;
	unsigned long int  ulnum;
	float              realnum;
	double             lrealnum;
	char              *string;
	yysyntaxtree       tree;
}

%type <tree> error
%type <tree> graph
%type <tree> graph_entry_list
%type <tree> graph_entry
%type <tree> graph_attribute
%type <tree> enum_color
%type <tree> enum_topsort
%type <tree> enum_orientation
%type <tree> enum_layoutalgorithm
%type <tree> enum_layoutfrequency
%type <tree> enum_status
%type <tree> enum_yes_no
%type <tree> enum_cross_weight
%type <tree> enum_view
%type <tree> enum_arrow_mode
%type <tree> foldnode_defaults
%type <tree> foldedge_defaults
%type <tree> node_defaults
%type <tree> edge_defaults
%type <tree> node
%type <tree> node_attribute_list
%type <tree> edge
%type <tree> nearedge
%type <tree> bentnearedge
%type <tree> backedge
%type <tree> edge_attribute_list
%type <tree> constraint
%type <tree> constraint_attribute_list
%type <tree> node_attribute
%type <tree> enum_textmode
%type <tree> enum_shape
%type <tree> enum_node_align
%type <tree> enum_iconstyle
%type <tree> edge_attribute
%type <tree> enum_linestyle
%type <tree> enum_arrowstyle
%type <tree> constraint_attribute
%type <tree> string_array
%type <tree> enum_name
%type <tree> enum_dimension
%type <tree> attribute_value
%type <tree> array_value
%type <tree> index_value_list
%type <tree> index_value
%type <tree> range
%type <tree> index
%type <tree> int_const
%type <tree> float_const
%type <tree> str_const
%type <lnum> LEX_INT
%type <lnum> LEX_STRING
%type <byte> LEX_CHAR
%type <lrealnum> LEX_FLOAT


%start graph


%{
/************* SECOND USER DECLARATIONS *****/
%}

%%


graph		: LEXWORD_GRAPH '{' graph_entry_list '}'
					{
		 $$ = REVERT($3); Syntax_Tree = $$; }
		;


graph_entry_list: graph_entry_list graph_entry
					{
		 $$ = T_graph_entry($2,$1,yylocate(@1));   }
		| graph_entry
					{
		 $$ = T_graph_entry($1,NULL,yylocate(@1)); }
		;


graph_entry	: graph_attribute	{
		 $$ = T_graph_attribute($1,yylocate(@1)); }
		| node_defaults		{
		 $$ = T_node_defaults($1,yylocate(@1));   }
		| edge_defaults		{
		 $$ = T_edge_defaults($1,yylocate(@1));   }
		| foldnode_defaults	{
		 $$ = T_foldnode_defaults($1,yylocate(@1));}
		| foldedge_defaults	{
		 $$ = T_foldedge_defaults($1,yylocate(@1));}
		| graph			{
		 $$ = T_graph($1,yylocate(@1));           }
		| node			{
		 $$ = T_node($1,yylocate(@1));            }
		| edge			{
		 $$ = T_edge($1,yylocate(@1));            }
		| nearedge		{
		 $$ = T_near_edge($1,yylocate(@1));       }
		| bentnearedge		{
		 $$ = T_bent_near_edge($1,yylocate(@1));  }
		| backedge		{
		 $$ = T_back_edge($1,yylocate(@1));       }
		| constraint		{
		 $$ = T_constraint($1,yylocate(@1));      }
		;


graph_attribute	: LEXWORD_TITLE ':' str_const
					{
		 $$ = T_title($3,yylocate(@1));              }
		| LEXWORD_LABEL ':' str_const
					{
		 $$ = T_label($3,yylocate(@1));              }
		| LEXWORD_INFO1 ':' str_const
					{
		 $$ = T_info1($3,yylocate(@1));              }
		| LEXWORD_INFO2 ':' str_const
					{
		 $$ = T_info2($3,yylocate(@1));              }
		| LEXWORD_INFO3 ':' str_const
					{
		 $$ = T_info3($3,yylocate(@1));              }
		| LEXWORD_COLOR ':' enum_color
					{
		 $$ = T_color($3,yylocate(@1));              }
		| LEXWORD_TEXTCOLOR ':'enum_color
					{
		 $$ = T_textcolor($3,yylocate(@1));          }
                | LEXWORD_BORDERCOLOR ':'enum_color
					{
		 $$ = T_colorborder($3,yylocate(@1));        }
                | LEXWORD_WIDTH ':' int_const
					{
		 $$ = T_width($3,yylocate(@1));              }
                | LEXWORD_HEIGHT ':' int_const
					{
		 $$ = T_height($3,yylocate(@1));             }
                | LEXWORD_BORDERWIDTH ':' int_const
					{
		 $$ = T_borderwidth($3,yylocate(@1));        }
		| 'x' ':' int_const	{
		 $$ = T_xdef($3,yylocate(@1));               }
		| 'y' ':' int_const	{
		 $$ = T_ydef($3,yylocate(@1));               }
		| LEXWORD_LOC '{' 'x' ':' int_const 'y' ':' int_const '}'
					{
		 $$ = T_loc($5,$8,yylocate(@1));       }
		| LEXWORD_FOLDING ':' int_const
					{
		 $$ = T_folding($3,yylocate(@1));            }
		| LEXWORD_SCALING ':' float_const
					{
		 $$ = T_scaling($3,yylocate(@1));            }
		| LEXWORD_SHRINK ':' int_const
					{
		 $$ = T_shrink($3,yylocate(@1));             }
		| LEXWORD_STRETCH ':' int_const
					{
		 $$ = T_stretch($3,yylocate(@1));            }
		| LEXWORD_TEXTMODE ':' enum_textmode
					{
		 $$ = T_textmode($3,yylocate(@1));           }
		| LEXWORD_SHAPE ':' enum_shape
					{
		 $$ = T_shape($3,yylocate(@1));    	       }
                | LEXWORD_LEVEL ':' int_const
					{
		 $$ = T_level($3,yylocate(@1));              }
                | LEXWORD_VORDER ':' int_const
					{
		 $$ = T_level($3,yylocate(@1));              }
                | LEXWORD_HORDER ':' int_const
					{
		 $$ = T_horizontal_order($3,yylocate(@1));   }
                | LEXWORD_STATUS ':' enum_status
					{
		 $$ = T_status($3,yylocate(@1));             }

                | LEXWORD_XMAX ':' int_const
					{
		 $$ = T_xmax($3,yylocate(@1));               }
                | LEXWORD_YMAX ':' int_const
					{
		 $$ = T_ymax($3,yylocate(@1));               }
                | LEXWORD_XBASE ':' int_const
					{
		 $$ = T_xbase($3,yylocate(@1));              }
                | LEXWORD_YBASE ':' int_const
					{
		 $$ = T_ybase($3,yylocate(@1));              }
                | LEXWORD_XSPACE ':' int_const
					{
		 $$ = T_xspace($3,yylocate(@1));             }
                | LEXWORD_XLSPACE ':' int_const
					{
		 $$ = T_xlspace($3,yylocate(@1));            }
                | LEXWORD_YSPACE ':' int_const
					{
		 $$ = T_yspace($3,yylocate(@1));             }
                | LEXWORD_XRASTER ':' int_const
					{
		 $$ = T_xraster($3,yylocate(@1));            }
                | LEXWORD_XLRASTER ':' int_const
					{
		 $$ = T_xlraster($3,yylocate(@1));           }
                | LEXWORD_YRASTER ':' int_const
					{
		 $$ = T_yraster($3,yylocate(@1));            }
		| LEXWORD_INVISIBLE ':' int_const
					{
		 $$ = T_hidden($3,yylocate(@1)); 	       }
		| LEXWORD_HIDDEN ':' int_const
					{
		 $$ = T_hidden($3,yylocate(@1)); 	       }
		| LEXWORD_CLASSNAME int_const ':' str_const
					{
		 $$ = T_classname($2,$4,yylocate(@1));       }
		| LEXWORD_INFONAME int_const ':' str_const
					{
		 $$ = T_infoname($2,$4,yylocate(@1));        }
                | LEXWORD_COLORENTRY int_const ':' int_const int_const int_const
					{
		 $$ = T_colentry($2,$4,$5,$6,yylocate(@1));   }

                | LEXWORD_LAYOUTALGORITHM ':' enum_layoutalgorithm
					{
		 $$ = T_layoutalgorithm($3,yylocate(@1));    }
		| LEXWORD_LAYOUTFREQUENCY ':' enum_layoutfrequency
					{
		 $$ = T_layoutfrequency($3,yylocate(@1));    }
                | LEXWORD_LAYOUTDOWNFACTOR ':' int_const
					{
		 $$ = T_downfactor($3,yylocate(@1));         }
                | LEXWORD_LAYOUTUPFACTOR ':' int_const
					{
		 $$ = T_upfactor($3,yylocate(@1));           }
                | LEXWORD_LAYOUTNEARFACTOR ':' int_const
					{
		 $$ = T_nearfactor($3,yylocate(@1));         }
                | LEXWORD_LAYOUTSPLINEFACTOR ':' int_const
					{
		 $$ = T_splinefactor($3,yylocate(@1));       }

                | LEXWORD_LATE_LABELS ':' enum_yes_no
					{
		 $$ = T_late_edge_label($3,yylocate(@1));    }
                | LEXWORD_DISPLAY_EDGE_LABELS ':' enum_yes_no
					{
		 $$ = T_display_edge_label($3,yylocate(@1)); }
                | LEXWORD_DIRTY_EDGE_LABELS ':' enum_yes_no
					{
		 $$ = T_dirty_edge_label($3,yylocate(@1));   }
		| LEXWORD_FINETUNING ':' enum_yes_no
				 	{
		 $$ = T_finetuning($3,yylocate(@1));	       }
                | LEXWORD_HIDESINGLES  ':' enum_yes_no
					{
		 $$ = T_hidesingles($3,yylocate(@1));        }
		| LEXWORD_STRAIGHTPHASE ':'  enum_yes_no
				 	{
		 $$ = T_straightphase($3,yylocate(@1));	       }
		| LEXWORD_PRIORITYPHASE ':'  enum_yes_no
				 	{
		 $$ = T_priophase($3,yylocate(@1));	       }
		| LEXWORD_MANHATTEN ':'  enum_yes_no
				 	{
		 $$ = T_manhatten($3,yylocate(@1));	       }
		| LEXWORD_SMANHATTEN ':'  enum_yes_no
				 	{
		 $$ = T_smanhatten($3,yylocate(@1));	       }
		| LEXWORD_NONEAREDGES	{
		 $$ = T_nonearedges(yylocate(@1));	       }
		| LEXWORD_NEAREDGES ':' LEXWORD_NO
					{
		 $$ = T_nonearedges(yylocate(@1));	       }
		| LEXWORD_NEAREDGES ':' LEXWORD_YES
					{
		 $$ = T_dummy(yylocate(@1));	     	       }
                | LEXWORD_ORIENTATION ':' enum_orientation
					{
		 $$ = T_orientation($3,yylocate(@1));        }
                | LEXWORD_NODE_ALIGN ':' enum_node_align
					{
		 $$ = T_node_alignment($3,yylocate(@1));     }
                | LEXWORD_PORTSHARING ':' enum_yes_no
					{
		 $$ = T_port_sharing($3,yylocate(@1));       }
                | LEXWORD_ARROWMODE  ':' enum_arrow_mode
					{
		 $$ = T_arrow_mode($3,yylocate(@1));         }

                | LEXWORD_SPREADLEVEL ':' int_const
					{
		 $$ = T_spreadlevel($3,yylocate(@1));        }
                | LEXWORD_TREEFACTOR ':' float_const
					{
		 $$ = T_treefactor($3,yylocate(@1));         }

                | LEXWORD_CROSSING_P2 ':' enum_yes_no
					{
		 $$ = T_crossing_phase2($3,yylocate(@1));    }
                | LEXWORD_CROSSING_OPT ':' enum_yes_no
					{
		 $$ = T_crossing_opt($3,yylocate(@1));       }
                | LEXWORD_CROSSING_WEIGHT ':' enum_cross_weight
					{
		 $$ = T_crossing_weight($3,yylocate(@1));    }

                | LEXWORD_VIEW ':'  enum_view
					{
		 $$ = T_view_method($3,yylocate(@1));       	}
                | LEXWORD_EDGES ':' enum_yes_no
					{
		 $$ = T_view_edges($3,yylocate(@1));       	}
                | LEXWORD_NODES ':' enum_yes_no
					{
		 $$ = T_view_nodes($3,yylocate(@1));       	}
		| LEXWORD_SPLINES ':'    enum_yes_no
				 	{
		 $$ = T_view_splines($3,yylocate(@1));  	}

                | LEXWORD_BMAX ':' int_const
					{
		 $$ = T_bend_max($3,yylocate(@1));   		}
                | LEXWORD_CMAX ':' int_const
					{
		 $$ = T_cross_max($3,yylocate(@1));  		}
                | LEXWORD_CMIN ':' int_const
					{
		 $$ = T_cross_min($3,yylocate(@1));  		}
                | LEXWORD_PMAX ':' int_const
					{
		 $$ = T_pendel_max($3,yylocate(@1)); 		}
                | LEXWORD_PMIN ':' int_const
					{
		 $$ = T_pendel_min($3,yylocate(@1));  	}
                | LEXWORD_RMAX ':' int_const
					{
		 $$ = T_rubber_max($3,yylocate(@1)); 		}
                | LEXWORD_RMIN ':' int_const
					{
		 $$ = T_rubber_min($3,yylocate(@1));  	}
                | LEXWORD_SMAX ':' int_const
					{
		 $$ = T_straight_max($3,yylocate(@1)); 	}

                | LEXWORD_TYPENAME ':' str_const
					{
		 $$ = T_typename($3,yylocate(@1));           }
                | LEXWORD_INCLUDE ':' str_const
			 		{
		 $$ = T_include($3,yylocate(@1));            }
		| LEXWORD_LAYOUTPARAMETER ':' array_value
					{
		 $$ = T_layoutparameter($3,yylocate(@1));    }
		| LEXWORD_TOPSORT ':' enum_topsort
					{
		 $$ = T_topsort($3,yylocate(@1));            }
                | LEXWORD_INPUTFUNCTION ':' str_const
					{
		 $$ = T_inputfunction($3,yylocate(@1));      }
                | LEXWORD_OUTPUTFUNCTION ':' str_const
					{
		 $$ = T_outputfunction($3,yylocate(@1));     }
                | LEXWORD_XSCROLLBAR ':' int_const
					{
		 $$ = T_xscrollbar($3,yylocate(@1));         }
                | LEXWORD_YSCROLLBAR ':' int_const
					{
		 $$ = T_yscrollbar($3,yylocate(@1));         }
		;


enum_color	: LEXWORD_AQUAMARINE	{
		 $$ = T_aquamarine(yylocate(@1));   }
		| LEXWORD_BLACK		{
		 $$ = T_black(yylocate(@1));        }
		| LEXWORD_BLUE		{
		 $$ = T_blue(yylocate(@1));         }
		| LEXWORD_CYAN		{
		 $$ = T_cyan(yylocate(@1));         }
		| LEXWORD_DARKBLUE	{
		 $$ = T_darkblue(yylocate(@1));     }
		| LEXWORD_DARKCYAN	{
		 $$ = T_darkcyan(yylocate(@1));     }
		| LEXWORD_DARKGREEN	{
		 $$ = T_darkgreen(yylocate(@1));    }
		| LEXWORD_DARKGREY	{
		 $$ = T_darkgrey(yylocate(@1));     }
		| LEXWORD_DARKMAGENTA	{
		 $$ = T_darkmagenta(yylocate(@1));  }
		| LEXWORD_DARKRED	{
		 $$ = T_darkred(yylocate(@1));      }
		| LEXWORD_DARKYELLOW	{
		 $$ = T_darkyellow(yylocate(@1));   }
		| LEXWORD_GOLD		{
		 $$ = T_gold(yylocate(@1));         }
		| LEXWORD_GREEN		{
		 $$ = T_green(yylocate(@1));        }
		| LEXWORD_KHAKI		{
		 $$ = T_khaki(yylocate(@1));        }
		| LEXWORD_LIGHTBLUE	{
		 $$ = T_lightblue(yylocate(@1));    }
		| LEXWORD_LIGHTCYAN	{
		 $$ = T_lightcyan(yylocate(@1));    }
		| LEXWORD_LIGHTGREEN	{
		 $$ = T_lightgreen(yylocate(@1));   }
		| LEXWORD_LIGHTGREY	{
		 $$ = T_lightgrey(yylocate(@1));    }
		| LEXWORD_LIGHTMAGENTA	{
		 $$ = T_lightmagenta(yylocate(@1)); }
		| LEXWORD_LIGHTRED	{
		 $$ = T_lightred(yylocate(@1));     }
		| LEXWORD_LIGHTYELLOW	{
		 $$ = T_lightyellow(yylocate(@1));  }
		| LEXWORD_LILAC		{
		 $$ = T_lilac(yylocate(@1));        }
		| LEXWORD_MAGENTA	{
		 $$ = T_magenta(yylocate(@1));      }
		| LEXWORD_ORANGE	{
		 $$ = T_orange(yylocate(@1));       }
		| LEXWORD_ORCHID	{
		 $$ = T_orchid(yylocate(@1));       }
		| LEXWORD_PINK		{
		 $$ = T_pink(yylocate(@1));         }
		| LEXWORD_PURPLE	{
		 $$ = T_purple(yylocate(@1));       }
		| LEXWORD_RED		{
		 $$ = T_red(yylocate(@1));          }
		| LEXWORD_TURQUOISE	{
		 $$ = T_turquoise(yylocate(@1));    }
		| LEXWORD_WHITE		{
		 $$ = T_white(yylocate(@1));        }
		| LEXWORD_YELLOW	{
		 $$ = T_yellow(yylocate(@1));       }
		| LEXWORD_YELLOWGREEN	{
		 $$ = T_yellowgreen(yylocate(@1));  }
                | int_const 		{
		 $$ = T_colindex($1,yylocate(@1));   }
		;


enum_topsort	: LEXWORD_HIGH		{
		 $$ = T_high(yylocate(@1)); 	       }
		| LEXWORD_LOW 		{
		 $$ = T_low(yylocate(@1));          }
		;


enum_orientation: LEXWORD_TOP_TO_BOTTOM	{
		 $$ = T_top_to_bottom(yylocate(@1)); }
		| LEXWORD_BOTTOM_TO_TOP	{
		 $$ = T_bottom_to_top(yylocate(@1)); }
		| LEXWORD_LEFT_TO_RIGHT	{
		 $$ = T_left_to_right(yylocate(@1)); }
		| LEXWORD_RIGHT_TO_LEFT	{
		 $$ = T_right_to_left(yylocate(@1)); }
		;


enum_layoutalgorithm:
		  LEXWORD_BARYCENTER 	{
		 $$ = T_barycenter(yylocate(@1));    }
		| LEXWORD_ISI 		{
		 $$ = T_isi(yylocate(@1));           }
		| LEXWORD_PLANAR 	{
		 $$ = T_planar(yylocate(@1));        }
		| LEXWORD_CONSTRAINTS 	{
		 $$ = T_constaints(yylocate(@1));    }

		| LEXWORD_TREE 		{
		 $$ = T_tree(yylocate(@1));          }
		| LEXWORD_MAXDEPTH	{
		 $$ = T_maxdepth(yylocate(@1));      }
		| LEXWORD_MINDEPTH	{
		 $$ = T_mindepth(yylocate(@1));      }
		| LEXWORD_MAXDEPTHSLOW	{
		 $$ = T_maxdepthslow(yylocate(@1));  }
		| LEXWORD_MINDEPTHSLOW	{
		 $$ = T_mindepthslow(yylocate(@1));  }
		| LEXWORD_MAXDEGREE   	{
		 $$ = T_maxdegree(yylocate(@1));     }
		| LEXWORD_MINDEGREE   	{
		 $$ = T_mindegree(yylocate(@1));     }
		| LEXWORD_MAXINDEGREE  	{
		 $$ = T_maxindegree(yylocate(@1));   }
		| LEXWORD_MININDEGREE  	{
		 $$ = T_minindegree(yylocate(@1));   }
		| LEXWORD_MAXOUTDEGREE 	{
		 $$ = T_maxoutdegree(yylocate(@1));  }
		| LEXWORD_MINOUTDEGREE 	{
		 $$ = T_minoutdegree(yylocate(@1));  }
		| LEXWORD_MINBACK 	{
		 $$ = T_minbackwards(yylocate(@1));  }
		| LEXWORD_DFS     	{
		 $$ = T_depthfirst(yylocate(@1));    }
		;


enum_layoutfrequency:
		  LEXWORD_EVERY 	{
		 $$ = T_every(yylocate(@1)); 	}
		| LEXWORD_MANUAL 	{
		 $$ = T_manual(yylocate(@1)); 	}
		;


enum_status 	: LEXWORD_BLACK 	{
		 $$ = T_black(yylocate(@1)); 	}
		| LEXWORD_GREY 		{
		 $$ = T_grey(yylocate(@1)); 		}
		| LEXWORD_WHITE 	{
		 $$ = T_white(yylocate(@1)); 	}
		;


enum_yes_no	: LEXWORD_YES 		{
		 $$ = T_yes(yylocate(@1)); 		}
		| LEXWORD_NO 		{
		 $$ = T_no(yylocate(@1)); 		}
		;

enum_cross_weight : LEXWORD_BARY	{
		 $$ = T_bary(yylocate(@1)); 	       }
		  | LEXWORD_MEDIAN	{
		 $$ = T_median(yylocate(@1));       }
		  | LEXWORD_BARYMEDIAN	{
		 $$ = T_barymedian(yylocate(@1));   }
		  | LEXWORD_MEDIANBARY	{
		 $$ = T_medianbary(yylocate(@1));   }
		  ;

enum_view	: LEXWORD_CFISH         {
		 $$ = T_cfish(yylocate(@1)); 	}
		| LEXWORD_FCFISH	{
		 $$ = T_fcfish(yylocate(@1));	}
		| LEXWORD_PFISH		{
		 $$ = T_pfish(yylocate(@1)); 	}
		| LEXWORD_FPFISH	{
		 $$ = T_fpfish(yylocate(@1));	}
		;

enum_arrow_mode	: LEXWORD_FIXED		{
		 $$ = T_fixed(yylocate(@1));         }
		| LEXWORD_FREE 		{
		 $$ = T_free(yylocate(@1));          }
		;

foldnode_defaults: LEXWORD_FOLDNODE node_attribute
					{
		 $$ = $2;              }
		;

foldedge_defaults: LEXWORD_FOLDEDGE edge_attribute
					{
		 $$ = $2;              }
		;

node_defaults	: LEXWORD_NODE1 node_attribute
					{
		 $$ = $2;              }
		;

edge_defaults   : LEXWORD_EDGE1 edge_attribute
					{
		 $$ = $2;              }
                ;


node		: LEXWORD_NODE2 '{' node_attribute_list '}'
					{
		 $$ = REVERT($3); }
		;


node_attribute_list: node_attribute_list node_attribute
					{
		 $$ = T_node_attribute($2,$1,yylocate(@1));   }
		   | node_attribute
					{
		 $$ = T_node_attribute($1,NULL,yylocate(@1)); }
		   ;


edge            : LEXWORD_EDGE2 '{' edge_attribute_list '}'
					{
		 $$ = REVERT($3); }
                ;

nearedge	: LEXWORD_NEAREDGE '{' edge_attribute_list '}'
					{
		 $$ = REVERT($3); }
                ;

bentnearedge	: LEXWORD_BENTNEAREDGE '{' edge_attribute_list '}'
					{
		 $$ = REVERT($3); }
                ;

backedge	: LEXWORD_BACKEDGE '{' edge_attribute_list '}'
					{
		 $$ = REVERT($3); }
                ;


edge_attribute_list: edge_attribute_list edge_attribute
		    			{
		 $$ = T_edge_attribute($2,$1,yylocate(@1));   }
                   | edge_attribute
					{
		 $$ = T_edge_attribute($1,NULL,yylocate(@1)); }
                   ;


constraint 	: LEXWORD_CONSTRAINT '{' constraint_attribute_list '}'
					{
		 $$ = REVERT($3); }
                ;


constraint_attribute_list:
		  constraint_attribute_list constraint_attribute
					{
		 $$ = T_constraint_attribute($2,$1,yylocate(@1));   }
                | constraint_attribute
					{
		 $$ = T_constraint_attribute($1,NULL,yylocate(@1)); }
                   ;


node_attribute	: LEXWORD_TITLE ':' str_const
					{
		 $$ = T_title($3,yylocate(@1));        }
		| LEXWORD_LABEL ':' str_const
					{
		 $$ = T_label($3,yylocate(@1));        }
		| LEXWORD_INFO1 ':' str_const
					{
		 $$ = T_info1($3,yylocate(@1));        }
		| LEXWORD_INFO2 ':' str_const
					{
		 $$ = T_info2($3,yylocate(@1));        }
		| LEXWORD_INFO3 ':' str_const
					{
		 $$ = T_info3($3,yylocate(@1));        }
                | LEXWORD_FONTNAME ':' str_const
					{
		 $$ = T_fontname($3,yylocate(@1));     }
                | LEXWORD_COLOR ':' enum_color
					{
		 $$ = T_color($3,yylocate(@1));        }
		| LEXWORD_TEXTCOLOR ':'enum_color
					{
		 $$ = T_textcolor($3,yylocate(@1));    }
                | LEXWORD_BORDERCOLOR ':'enum_color
					{
		 $$ = T_bordercolor($3,yylocate(@1));  }
                | LEXWORD_ICONFILE ':' str_const
					{
		 $$ = T_iconfile($3,yylocate(@1));     }
                | LEXWORD_ANCHORPOINTS ':' str_const
					{
		 $$ = T_anchorpoints($3,yylocate(@1)); }
                | LEXWORD_TYPENAME ':' str_const
					{
		 $$ = T_typename($3,yylocate(@1));     }
		| LEXWORD_WIDTH ':' int_const
					{
		 $$ = T_width($3,yylocate(@1));        }
		| LEXWORD_HEIGHT ':' int_const
					{
		 $$ = T_height($3,yylocate(@1));       }
                | LEXWORD_BORDERWIDTH ':' int_const
					{
		 $$ = T_borderwidth($3,yylocate(@1));  }
		| LEXWORD_LOC '{' 'x' ':' int_const 'y' ':' int_const '}'
					{
		 $$ = T_loc($5,$8,yylocate(@1));       }
		| LEXWORD_FOLDING ':' int_const
					{
		 $$ = T_folding($3,yylocate(@1));      }
		| LEXWORD_SCALING ':' float_const
					{
		 $$ = T_scaling($3,yylocate(@1));            }
		| LEXWORD_SHRINK ':' int_const
					{
		 $$ = T_shrink($3,yylocate(@1));       }
		| LEXWORD_STRETCH ':' int_const
					{
		 $$ = T_stretch($3,yylocate(@1));      }
                | LEXWORD_ICONWIDTH ':' int_const
					{
		 $$ = T_iconwidth($3,yylocate(@1));    }
                | LEXWORD_ICONHEIGHT ':' int_const
					{
		 $$ = T_iconheight($3,yylocate(@1));   }
		| LEXWORD_TEXTMODE ':' enum_textmode
					{
		 $$ = T_textmode($3,yylocate(@1));     }
		| LEXWORD_ICONSTYLE ':' enum_iconstyle
					{
		 $$ = T_iconstyle($3,yylocate(@1));    }
		| LEXWORD_SHAPE ':' enum_shape
					{
		 $$ = T_shape($3,yylocate(@1));    	 }
                | LEXWORD_LEVEL ':' int_const
					{
		 $$ = T_level($3,yylocate(@1));        }
                | LEXWORD_VORDER ':' int_const
					{
		 $$ = T_level($3,yylocate(@1));            }
                | LEXWORD_HORDER ':' int_const
					{
		 $$ = T_horizontal_order($3,yylocate(@1)); }
		;

enum_textmode	: LEXWORD_CENTER 	{
		 $$ = T_center(yylocate(@1));        }
		| LEXWORD_LEFT_JUSTIFY 	{
		 $$ = T_left_justify(yylocate(@1));  }
		| LEXWORD_RIGHT_JUSTIFY	{
		 $$ = T_right_justify(yylocate(@1)); }
		;

enum_shape	: LEXWORD_BOX		{
		 $$ = T_box(yylocate(@1)); 		}
		| LEXWORD_RHOMB		{
		 $$ = T_rhomb(yylocate(@1));		}
		| LEXWORD_ELLIPSE	{
		 $$ = T_ellipse(yylocate(@1));	}
		| LEXWORD_TRIANGLE	{
		 $$ = T_triangle(yylocate(@1));	}
		;

enum_node_align	: LEXWORD_BOTTOM	{
		 $$ = T_bottom(yylocate(@1)); }
		| LEXWORD_TOP 		{
		 $$ = T_top(yylocate(@1));    }
		| LEXWORD_CENTER 	{
		 $$ = T_center(yylocate(@1)); }
		;

enum_iconstyle	: LEXWORD_BOTTOM	{
		 $$ = T_bottom(yylocate(@1)); }
		| LEXWORD_TOP 		{
		 $$ = T_top(yylocate(@1));    }
		| LEXWORD_AROUND 	{
		 $$ = T_around(yylocate(@1)); }
		;


edge_attribute  : LEXWORD_SOURCENAME ':' str_const
					{
		 $$ = T_sourcename($3,yylocate(@1));    }
		| LEXWORD_TARGETNAME ':' str_const
					{
		 $$ = T_targetname($3,yylocate(@1));    }
                | LEXWORD_LABEL ':' str_const
					{
		 $$ = T_label($3,yylocate(@1));         }
		| LEXWORD_TEXTCOLOR ':'enum_color
					{
		 $$ = T_textcolor($3,yylocate(@1));          }
                | LEXWORD_FONTNAME ':' str_const
					{
		 $$ = T_fontname($3,yylocate(@1));      }
                | LEXWORD_COLOR ':' enum_color
					{
		 $$ = T_color($3,yylocate(@1));         }
                | LEXWORD_TYPENAME ':' str_const
					{
		 $$ = T_typename($3,yylocate(@1));      }
		| LEXWORD_THICKNESS ':' int_const
					{
		 $$ = T_thickness($3,yylocate(@1));     }
		| LEXWORD_CLASS ':' int_const
					{
		 $$ = T_class($3,yylocate(@1));         }
		| LEXWORD_PRIORITY ':' int_const
					{
		 $$ = T_priority($3,yylocate(@1));      }
		| LEXWORD_ARROWWIDTH ':' int_const
					{
		 $$ = T_arrowwidth($3,yylocate(@1));    }
                | LEXWORD_ARROWHEIGHT ':' int_const
					{
		 $$ = T_arrowheight($3,yylocate(@1));   }
                | LEXWORD_ARROWCOLOR ':' enum_color
					{
		 $$ = T_arrowcolor($3,yylocate(@1));    }
                | LEXWORD_BARROWCOLOR ':' enum_color
					{
		 $$ = T_barrowcolor($3,yylocate(@1));    }
		| LEXWORD_ARROWSIZE ':' int_const
					{
		 $$ = T_arrowsize($3,yylocate(@1));     }
		| LEXWORD_BARROWSIZE ':' int_const
					{
		 $$ = T_barrowsize($3,yylocate(@1));     }
		| LEXWORD_ARROWSTYLE ':' enum_arrowstyle
					{
		 $$ = T_arrowstyle($3,yylocate(@1));    }
		| LEXWORD_BARROWSTYLE ':' enum_arrowstyle
					{
		 $$ = T_barrowstyle($3,yylocate(@1));    }
		| LEXWORD_LINESTYLE ':' enum_linestyle
					{
		 $$ = T_linestyle($3,yylocate(@1));     }
		| LEXWORD_ANCHOR ':' int_const
					{
		 $$ = T_anchor($3,yylocate(@1));        }
                | LEXWORD_HORDER ':' int_const
					{
		 $$ = T_horizontal_order($3,yylocate(@1)); }
		;


enum_linestyle	: LEXWORD_CONTINUOUS 	{
		 $$ = T_continuous(yylocate(@1)); }
		| LEXWORD_SOLID 	{
		 $$ = T_continuous(yylocate(@1)); }
		| LEXWORD_DOTTED 	{
		 $$ = T_dotted(yylocate(@1));     }
		| LEXWORD_DASHED 	{
		 $$ = T_dashed(yylocate(@1));     }
		| LEXWORD_INVISIBLE     {
		 $$ = T_invisible(yylocate(@1));  }
		;


enum_arrowstyle	: LEXWORD_NONE 		{
		 $$ = T_none(yylocate(@1));      }
		| LEXWORD_LINE 		{
		 $$ = T_line(yylocate(@1));      }
		| LEXWORD_SOLID 	{
		 $$ = T_solid(yylocate(@1));     }
		;

constraint_attribute  : LEXWORD_TITLE ':' str_const
					{
		 $$ = T_title($3,yylocate(@1));     }
		| LEXWORD_PRIORITY ':' int_const
					{
		 $$ = T_priority($3,yylocate(@1));  }
		| LEXWORD_SIZE ':' int_const
					{
		 $$ = T_size($3,yylocate(@1));      }
		| LEXWORD_NODES ':' '{' string_array '}'
					{
		 $$ = T_nodes($4,yylocate(@1));     }
		| LEXWORD_INTERVAL ':' array_value
					{
		 $$ = T_interval($3,yylocate(@1));  }
		| LEXWORD_NAME ':' enum_name
					{
		 $$ = T_name($3,yylocate(@1));      }
		| LEXWORD_DIMENSION ':' enum_dimension
					{
		 $$ = T_dimension($3,yylocate(@1)); }
		;

string_array	: string_array str_const
					{
		 $$ = T_string_array($1,$2,yylocate(@1));    }
		| str_const		{
		 $$ = T_string_array(NULL,$1,yylocate(@1));  }
		;

enum_name	: LEXWORD_EQUAL 	{
		 $$ = T_equal(yylocate(@1));            }
		| LEXWORD_SMALLER 	{
		 $$ = T_smaller(yylocate(@1));          }
		| LEXWORD_GREATER 	{
		 $$ = T_greater(yylocate(@1));          }
		| LEXWORD_NEIGHBORS 	{
		 $$ = T_neighbors(yylocate(@1));        }
		| LEXWORD_LOW_MARGIN 	{
		 $$ = T_low_margin(yylocate(@1));       }
		| LEXWORD_HIGH_MARGIN 	{
		 $$ = T_high_margin(yylocate(@1));      }
		| LEXWORD_RANGE 	{
		 $$ = T_xrange(yylocate(@1));           }
		| LEXWORD_CLUSTER 	{
		 $$ = T_cluster(yylocate(@1));          }
		| LEXWORD_LIMIT 	{
		 $$ = T_limit(yylocate(@1));            }
		| LEXWORD_ABOVE 	{
		 $$ = T_above(yylocate(@1));            }
		| LEXWORD_BELOW	 	{
		 $$ = T_below(yylocate(@1));            }
		| LEXWORD_LEFT 		{
		 $$ = T_left(yylocate(@1));             }
		| LEXWORD_RIGHT 	{
		 $$ = T_right(yylocate(@1));            }
		| LEXWORD_IN_FRONT 	{
		 $$ = T_in_font(yylocate(@1));          }
		| LEXWORD_BEHIND 	{
		 $$ = T_behind(yylocate(@1));           }
		| LEXWORD_EQUAL_POSITION{
		 $$ = T_equal_position(yylocate(@1));   }
		| LEXWORD_EQUAL_ROW 	{
		 $$ = T_equal_row(yylocate(@1));        }
		| LEXWORD_EQUAL_COLUMN 	{
		 $$ = T_equal_column(yylocate(@1));     }
		| LEXWORD_TOP_MARGIN 	{
		 $$ = T_top_margin(yylocate(@1));       }
		| LEXWORD_BOTTOM_MARGIN {
		 $$ = T_bottom_margin(yylocate(@1));    }
		| LEXWORD_LEFT_MARGIN 	{
		 $$ = T_left_margin(yylocate(@1));      }
		| LEXWORD_RIGHT_MARGIN 	{
		 $$ = T_right_margin(yylocate(@1));     }
		| LEXWORD_UPPER_NEIGHBOR{
		 $$ = T_upper_neighbor(yylocate(@1));   }
		| LEXWORD_LOWER_NEIGHBOR{
		 $$ = T_lower_neighbor(yylocate(@1));   }
		| LEXWORD_LEFT_NEIGHBOR {
		 $$ = T_left_neighbor(yylocate(@1));    }
		| LEXWORD_RIGHT_NEIGHBOR{
		 $$ = T_right_neighbor(yylocate(@1));   }
		;

enum_dimension	: 'x' 			{
		 $$ = T_x(yylocate(@1)); }
		| 'y' 			{
		 $$ = T_y(yylocate(@1)); }
		| 'z' 			{
		 $$ = T_z(yylocate(@1)); }
		;

attribute_value	: LEX_INT       	{
		 $$ = T_integer($1,yylocate(@1)); }
		| LEX_FLOAT         	{
		 $$ = T_float($1,yylocate(@1));   }
		| LEX_CHAR          	{
		 $$ = T_char($1,yylocate(@1));    }
		| LEX_STRING   		{
		 $$ = T_string($1,yylocate(@1));  }
		| array_value		{
		 $$ = $1; }
		;

array_value	: '{' index_value_list '}'
					{
		 $$ = REVERT($2); }
		;

index_value_list: index_value_list index_value
					{
		 $$ = T_index_value($2,$1,yylocate(@1));   }
		| index_value		{
		 $$ = T_index_value($1,NULL,yylocate(@1)); }
		;

index_value	: attribute_value 	{
		 $$ = $1;           }
		| index ':' attribute_value
					{
		 $$ = T_index($1,$3,yylocate(@1)); }
		| range ':' attribute_value
					{
		 $$ = T_range($1,$3,yylocate(@1)); }
		| '*' ':' attribute_value
					{
		 $$ = T_stern($3,yylocate(@1));    }
		;

range		: '[' int_const '-' int_const ']'
					{
		 $$ = T_range($2,$4,yylocate(@1)); }
		;

index		: LEX_INT		{
		 $$ = T_index_val($1,yylocate(@1));    }
		;

int_const	: LEX_INT		{
		 $$ = T_integer($1,yylocate(@1));  }
		;

float_const	: LEX_FLOAT		{
		 $$ = T_float($1,yylocate(@1));    }
		;

str_const	: LEX_STRING		{
		 $$ = T_string($1,yylocate(@1));   }
		;


%%




/*----------------------------*/
/* Main entry point of parser */
/*----------------------------*/

/*   returns the number of detected errors and binds the syntaxtree
 *   to the variable Syntax_Tree.
 */
int parse()
{
	nr_errors = 0;
	debugmessage("yyparse()\n", "");
	yyparse();

	return (nr_errors);
} /* parse */




