%{



/*--------------------------------------------------------------------*/
/*            A flex scanner specification made by parsegen           */
/*--------------------------------------------------------------------*/
 


typedef struct stree_node *syntaxtree;
#undef yysyntaxtree
#define yysyntaxtree syntaxtree



#define BISONGEN
#undef  YACCGEN

#include <stdio.h>

#include <string.h>

#include "y.tab.h"


/*
 *   Copyright (C) 1993--1995 by Georg Sander, Iris Lemke, and
 *                               the Compare Consortium 
 *
 *  This program and documentation is free software; you can redistribute 
 *  it under the terms of the  GNU General Public License as published by
 *  the  Free Software Foundation;  either version 2  of the License,  or
 *  (at your option) any later version.
 *
 *  This  program  is  distributed  in  the hope that it will be useful,
 *  but  WITHOUT ANY WARRANTY;  without  even  the  implied  warranty of
 *  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the
 *  GNU General Public License for more details.
 *
 *  You  should  have  received a copy of the GNU General Public License
 *  along  with  this  program;  if  not,  write  to  the  Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "globals.h"
#include "main.h"
#include "options.h"

#ifndef YY_CHAR
#define YY_CHAR char
extern YY_CHAR *yytext;
#endif

/* the following is added for flex 2.4.6 */

#ifndef YY_MALLOC_DECL
#define YY_MALLOC_DECL
#include <malloc.h>
#endif
 
#undef YYVALGLOBAL
 
extern yysyntaxtree Syntax_Tree;

#define exit(a) gs_exit(a)
extern void gs_exit            _PP((int x));
 
void   line_directive _PP((char *text));
void   escape_transl  _PP((char *text));
void   syntaxerror    _PP((int line,int pos,char *mesge));
void   warning        _PP((int line,int pos,char *mesge));
 
#ifndef yysyntaxerror
#define yysyntaxerror(l,p,m) syntaxerror(l,p,m)
#endif
 
#ifndef yyerror
#define yyerror(x) { \
        SPRINTF(message,"unexpected %s \"%s\" (%s)", \
                 ((yychar<0)?"(?)":yytokname[YYTRANSLATE(yychar)]),     \
                 (strlen(yytext)<48?yytext:"(?)"),x); \
        syntaxerror(line_nr,pos_nr,message);\
    }
#endif
 
/* Memory Management */

#ifdef MEMBLOCKSIZE
#define PARSEBLOCKSIZE (MEMBLOCKSIZE/sizeof(struct stree_node)+1)
#endif

/* Hash Table */

#define hash_size 22079
 




char * ParseMalloc(int x);
#undef PARSEGENSTD
#define PARSEGENSTD


 
#ifndef STDHASH
#define STDHASH


#undef  HASHGENSTD
#define HASHGENSTD


/*--------------------------------------------------------------------*/
/*  Standard Hash Table Routines                                      */
/*--------------------------------------------------------------------*/

/* Global Variables */
/*------------------*/

extern long   table_size;

#ifndef PARSEGENSTD

#ifndef ALIGN
#define ALIGN 8
#define IALIGN (ALIGN-1)
#endif
#ifndef STRINGBLOCKSIZE
#define STRINGBLOCKSIZE 5000
#endif

char *StringHeapMalloc(int x);
void StringHeapFree(void);

#endif /* PARSEGENSTD */

#ifndef hash_size
#define hash_size 211
#endif
#ifndef hashtable_size
#define hashtable_size 10000L
#endif


void FreeHash(void);
long HashInsert(register char *s);
long HashTableSize(void);
char *Decode(long x);

#endif /* STDHASH */

/*-- end of standard hash table interface ----------------------------*/
 




/*--------------------------------------------------------------------*/

/* Global Variables */
/*------------------*/

int line_nr;
int pos_nr;
extern int nr_errors;  /* from Scanner */

/* Local Variables */
/*-----------------*/

static char message[256];

#define SKIPYYTEXT { char *c; c = yytext; while (*c) { \
     if (*c == '\n') { pos_nr = 1; line_nr++; }  \
     else pos_nr++; c++; }}


/*--------------------------------------------------------------------*/
/*   Character classes                                                */
/*--------------------------------------------------------------------*/


%}


LETTER		[a-zA-Z]
DIGIT		[0-9]
INT		(\-)?{DIGIT}+
REAL		{INT}\.{INT}([E](\+|\-)?{INT})?
CHAR            '([^\\\']|\\.)+'
STRQ		[^\\\"]|\\.
STRING		\"({STRQ}*)\"
COMM		\/\*[^\*]*\*+([^\*\/][^\*]*\*+)*\/
COMME		\/\/[^\n]*
WSPACE          [ \t\f]*
LINEDIR1        {WSPACE}#{WSPACE}"line"?{WSPACE}{INT}{WSPACE}{STRING}.*\n
LINEDIR2        {WSPACE}#{WSPACE}"line"?{WSPACE}{INT}[^\"]*\n
PRAGMADIR       {WSPACE}#{WSPACE}"pragma".*\n



%{


#ifndef yywrap
/* Changed for flex 2.4.6 (static removed) */
int yywrap(void) { return (1); }
#endif


#ifndef RETURN
#ifndef YYLTYPE
#define RETURN(x) { 	pos_nr += strlen(yytext); \
			return(x); }
#else
extern YYLTYPE yylloc;
#define RETURN(x) { 	pos_nr += strlen(yytext); \
			yylloc.first_line = yylloc.last_line = line_nr; \
			yylloc.first_column = pos_nr-strlen(yytext);    \
			yylloc.last_column  = pos_nr;                   \
			return(x); }
#endif
#endif

#ifndef yysyntaxerror 
#define yysyntaxerror(l,p,m) {(void)printf("Line %d Pos %d: %s !\n",l,p,m);\
	nr_errors++;}
#endif

/*--------------------------------------------------------------------*/
/*   Token rules                                                      */
/*--------------------------------------------------------------------*/


%}

%%


"above"			{
		 RETURN(LEXWORD_ABOVE);               }
"anchor_points"		{
		 RETURN(LEXWORD_ANCHORPOINTS);        }
"anchorpoints"		{
		 RETURN(LEXWORD_ANCHORPOINTS);        }
"anchor"		{
		 RETURN(LEXWORD_ANCHOR);              }
"aquamarine"		{
		 RETURN(LEXWORD_AQUAMARINE);          }
"around"		{
		 RETURN(LEXWORD_AROUND);              }
"arrow_mode"		{
		 RETURN(LEXWORD_ARROWMODE);           }
"arrowmode"		{
		 RETURN(LEXWORD_ARROWMODE);           }
"arrow_height"		{
		 RETURN(LEXWORD_ARROWHEIGHT);         }
"arrowheight"		{
		 RETURN(LEXWORD_ARROWHEIGHT);         }
"arrow_width"		{
		 RETURN(LEXWORD_ARROWWIDTH);          }
"arrowwidth"		{
		 RETURN(LEXWORD_ARROWWIDTH);          }
"arrow_color"		{
		 RETURN(LEXWORD_ARROWCOLOR);          }
"arrowcolor"		{
		 RETURN(LEXWORD_ARROWCOLOR);          }
"arrow_style"		{
		 RETURN(LEXWORD_ARROWSTYLE);          }
"arrowstyle"		{
		 RETURN(LEXWORD_ARROWSTYLE);          }
"arrow_size"		{
		 RETURN(LEXWORD_ARROWSIZE);           }
"arrowsize"		{
		 RETURN(LEXWORD_ARROWSIZE);           }
"back_arrow_color"	{
		 RETURN(LEXWORD_BARROWCOLOR);         }
"backarrow_color"	{
		 RETURN(LEXWORD_BARROWCOLOR);         }
"backarrowcolor"	{
		 RETURN(LEXWORD_BARROWCOLOR);         }
"back_arrow_style"	{
		 RETURN(LEXWORD_BARROWSTYLE);         }
"backarrow_style"	{
		 RETURN(LEXWORD_BARROWSTYLE);         }
"backarrowstyle"	{
		 RETURN(LEXWORD_BARROWSTYLE);         }
"back_arrow_size"	{
		 RETURN(LEXWORD_BARROWSIZE);          }
"backarrow_size"	{
		 RETURN(LEXWORD_BARROWSIZE);          }
"backarrowsize"		{
		 RETURN(LEXWORD_BARROWSIZE);          }
"back_edge:"		{
		 RETURN(LEXWORD_BACKEDGE);            }
"backedge:"		{
		 RETURN(LEXWORD_BACKEDGE);            }
"bary_center"		{
		 RETURN(LEXWORD_BARYCENTER);          }
"barycenter"		{
		 RETURN(LEXWORD_BARYCENTER);          }
"bary"			{
		 RETURN(LEXWORD_BARY);                }
"barymedian"   	 	{
		 RETURN(LEXWORD_BARYMEDIAN);          } 
"behind"		{
		 RETURN(LEXWORD_BEHIND);              }
"below"			{
		 RETURN(LEXWORD_BELOW);               }
"black"			{
		 RETURN(LEXWORD_BLACK);               }
"blue"			{
		 RETURN(LEXWORD_BLUE);                }
"bmax"			{
		 RETURN(LEXWORD_BMAX);                }
"border_color"		{
		 RETURN(LEXWORD_BORDERCOLOR);         }
"bordercolor"		{
		 RETURN(LEXWORD_BORDERCOLOR);         }
"border_width"		{
		 RETURN(LEXWORD_BORDERWIDTH);         }
"borderwidth"		{
		 RETURN(LEXWORD_BORDERWIDTH);         }
"bottom_margin"		{
		 RETURN(LEXWORD_BOTTOM_MARGIN);       }
"bottommargin"		{
		 RETURN(LEXWORD_BOTTOM_MARGIN);       }
"bottomtotop"		{
		 RETURN(LEXWORD_BOTTOM_TO_TOP);       }
"bottom_to_top"		{
		 RETURN(LEXWORD_BOTTOM_TO_TOP);       }
"bottom"		{
		 RETURN(LEXWORD_BOTTOM);              }
"box"			{
		 RETURN(LEXWORD_BOX);                 }
"bent_near_edge:"	{
		 RETURN(LEXWORD_BENTNEAREDGE);        }
"bent_nearedge:"	{
		 RETURN(LEXWORD_BENTNEAREDGE);        }
"bentnearedge:"		{
		 RETURN(LEXWORD_BENTNEAREDGE);        }
"center"		{
		 RETURN(LEXWORD_CENTER);              }
"cfish"			{
		 RETURN(LEXWORD_CFISH);               }
"class_name"		{
		 RETURN(LEXWORD_CLASSNAME);           }
"classname"		{
		 RETURN(LEXWORD_CLASSNAME);           }
"class"			{
		 RETURN(LEXWORD_CLASS);               }
"cluster"		{
		 RETURN(LEXWORD_CLUSTER);             }
"cmin"			{
		 RETURN(LEXWORD_CMIN);                }
"cmax"			{
		 RETURN(LEXWORD_CMAX);                }
"color_entry"		{
		 RETURN(LEXWORD_COLORENTRY);          }
"colorentry"		{
		 RETURN(LEXWORD_COLORENTRY);          }
"color"			{
		 RETURN(LEXWORD_COLOR);               }
"constraints"		{
		 RETURN(LEXWORD_CONSTRAINTS);         }
"constraint:"		{
		 RETURN(LEXWORD_CONSTRAINT);          }
"continuous"		{
		 RETURN(LEXWORD_CONTINUOUS);          }
"crossing_weight"       {
		 RETURN(LEXWORD_CROSSING_WEIGHT);     }
"crossingweight"        {
		 RETURN(LEXWORD_CROSSING_WEIGHT);     }
"crossing_optimization" {
		 RETURN(LEXWORD_CROSSING_OPT);        }
"crossingoptimization"  {
		 RETURN(LEXWORD_CROSSING_OPT);        }
"crossing_phase2" 	{
		 RETURN(LEXWORD_CROSSING_P2);         }
"crossingphase2"  	{
		 RETURN(LEXWORD_CROSSING_P2);         }
"cyan"			{
		 RETURN(LEXWORD_CYAN);                }
"darkblue"		{
		 RETURN(LEXWORD_DARKBLUE);            }
"darkcyan"		{
		 RETURN(LEXWORD_DARKCYAN);            }
"darkgreen"		{
		 RETURN(LEXWORD_DARKGREEN);           }
"darkgray"		{
		 RETURN(LEXWORD_DARKGREY);            }
"darkgrey"		{
		 RETURN(LEXWORD_DARKGREY);            }
"darkmagenta"		{
		 RETURN(LEXWORD_DARKMAGENTA);         }
"darkred"		{
		 RETURN(LEXWORD_DARKRED);             }
"darkyellow"		{
		 RETURN(LEXWORD_DARKYELLOW);          }
"dashed"		{
		 RETURN(LEXWORD_DASHED);              } 
"depth_first_search"	{
		 RETURN(LEXWORD_DFS);   	       } 
"depthfirstsearch"	{
		 RETURN(LEXWORD_DFS);   	       } 
"dfs"			{
		 RETURN(LEXWORD_DFS);   	       } 
"dimension"		{
		 RETURN(LEXWORD_DIMENSION);           }
"dirtyedgelabels"	{
		 RETURN(LEXWORD_DIRTY_EDGE_LABELS);   }
"dirty_edge_labels"	{
		 RETURN(LEXWORD_DIRTY_EDGE_LABELS);   }
"displayedgelabels"	{
		 RETURN(LEXWORD_DISPLAY_EDGE_LABELS); }
"display_edge_labels"	{
		 RETURN(LEXWORD_DISPLAY_EDGE_LABELS); }
"dotted"		{
		 RETURN(LEXWORD_DOTTED);              }
"edge."			{
		 RETURN(LEXWORD_EDGE1);               }
"edge:"			{
		 RETURN(LEXWORD_EDGE2);               }
"edges"			{
		 RETURN(LEXWORD_EDGES);               }
"ellipse"		{
		 RETURN(LEXWORD_ELLIPSE);             }
"equalcolumn"		{
		 RETURN(LEXWORD_EQUAL_COLUMN);        }
"equal_column"		{
		 RETURN(LEXWORD_EQUAL_COLUMN);        }
"equalposition"		{
		 RETURN(LEXWORD_EQUAL_POSITION);      }
"equal_position"	{
		 RETURN(LEXWORD_EQUAL_POSITION);      }
"equalrow"		{
		 RETURN(LEXWORD_EQUAL_ROW);           }
"equal_row"		{
		 RETURN(LEXWORD_EQUAL_ROW);           }
"equal"			{
		 RETURN(LEXWORD_EQUAL);               }
"every"			{
		 RETURN(LEXWORD_EVERY);               }
"fcfish"		{
		 RETURN(LEXWORD_FCFISH);              }
"fpfish"		{
		 RETURN(LEXWORD_FPFISH);              }
"fixed"			{
		 RETURN(LEXWORD_FIXED);               }
"free"			{
		 RETURN(LEXWORD_FREE);                }
"fine_tuning"		{
		 RETURN(LEXWORD_FINETUNING);          }
"finetuning"		{
		 RETURN(LEXWORD_FINETUNING);          }
"fold_edge."		{
		 RETURN(LEXWORD_FOLDEDGE);            }
"foldedge."		{
		 RETURN(LEXWORD_FOLDEDGE);            }
"fold_node."		{
		 RETURN(LEXWORD_FOLDNODE);            }
"foldnode."		{
		 RETURN(LEXWORD_FOLDNODE);            }
"folding"		{
		 RETURN(LEXWORD_FOLDING);             }
"font_name"		{
		 RETURN(LEXWORD_FONTNAME);            }
"fontname"		{
		 RETURN(LEXWORD_FONTNAME);            }
"gold"			{
		 RETURN(LEXWORD_GOLD);                }
"graph:"		{
		 RETURN(LEXWORD_GRAPH);               }
"greater"		{
		 RETURN(LEXWORD_GREATER);             }
"green"			{
		 RETURN(LEXWORD_GREEN);               }
"gray"			{
		 RETURN(LEXWORD_GREY);                }
"grey"			{
		 RETURN(LEXWORD_GREY);                }
"height"		{
		 RETURN(LEXWORD_HEIGHT);              }
"hidesingles"		{
		 RETURN(LEXWORD_HIDESINGLES);         }
"hide_singles"		{
		 RETURN(LEXWORD_HIDESINGLES);         }
"highmargin"		{
		 RETURN(LEXWORD_HIGH_MARGIN);         }
"high_margin"		{
		 RETURN(LEXWORD_HIGH_MARGIN);         }
"high"			{
		 RETURN(LEXWORD_HIGH);		       }
"hidden"		{
		 RETURN(LEXWORD_HIDDEN);              }
"horizontalorder"	{
		 RETURN(LEXWORD_HORDER);              }
"horizontal_order"	{
		 RETURN(LEXWORD_HORDER);              }
"icon_file"		{
		 RETURN(LEXWORD_ICONFILE);            }
"iconfile"		{
		 RETURN(LEXWORD_ICONFILE);            }
"icon_height"		{
		 RETURN(LEXWORD_ICONHEIGHT);          }
"iconheight"		{
		 RETURN(LEXWORD_ICONHEIGHT);          }
"icon_style"		{
		 RETURN(LEXWORD_ICONSTYLE);           }
"iconstyle"		{
		 RETURN(LEXWORD_ICONSTYLE);           }
"icon_width"		{
		 RETURN(LEXWORD_ICONWIDTH);           }
"iconwidth"		{
		 RETURN(LEXWORD_ICONWIDTH);           }
"ignore_singles"	{
		 RETURN(LEXWORD_HIDESINGLES);         }
"ignoresingles"		{
		 RETURN(LEXWORD_HIDESINGLES);         }
"include"		{
		 RETURN(LEXWORD_INCLUDE);             }
"infoname"		{
		 RETURN(LEXWORD_INFONAME);            }
"info1"			{
		 RETURN(LEXWORD_INFO1);               }
"info2"			{
		 RETURN(LEXWORD_INFO2);               }
"info3"			{
		 RETURN(LEXWORD_INFO3);               }
"input_function"	{
		 RETURN(LEXWORD_INPUTFUNCTION);       }
"inputfunction"		{
		 RETURN(LEXWORD_INPUTFUNCTION);       }
"interval"		{
		 RETURN(LEXWORD_INTERVAL);            }
"invisible"		{
		 RETURN(LEXWORD_INVISIBLE);           }
"in_front"		{
		 RETURN(LEXWORD_IN_FRONT);            }
"isi"			{
		 RETURN(LEXWORD_ISI);                 }
"khaki"			{
		 RETURN(LEXWORD_KHAKI);               }
"label_color"		{
		 RETURN(LEXWORD_TEXTCOLOR);           }
"labelcolor"		{
		 RETURN(LEXWORD_TEXTCOLOR);           }
"label"			{
		 RETURN(LEXWORD_LABEL);               }
"lateedgelabels"     	{
		 RETURN(LEXWORD_LATE_LABELS);         }
"late_edge_labels"     	{
		 RETURN(LEXWORD_LATE_LABELS);         }
"layout_algorithm"	{
		 RETURN(LEXWORD_LAYOUTALGORITHM);     }
"layoutalgorithm"	{
		 RETURN(LEXWORD_LAYOUTALGORITHM);     }
"layout_frequency"	{
		 RETURN(LEXWORD_LAYOUTFREQUENCY);     }
"layoutfrequency"	{
		 RETURN(LEXWORD_LAYOUTFREQUENCY);     }
"layout_parameter"	{
		 RETURN(LEXWORD_LAYOUTPARAMETER);     }
"layoutparameter"	{
		 RETURN(LEXWORD_LAYOUTPARAMETER);     }
"layoutdownfactor"	{
		 RETURN(LEXWORD_LAYOUTDOWNFACTOR);    }
"layout_downfactor"	{
		 RETURN(LEXWORD_LAYOUTDOWNFACTOR);    }
"layoutupfactor"	{
		 RETURN(LEXWORD_LAYOUTUPFACTOR);      }
"layout_upfactor"	{
		 RETURN(LEXWORD_LAYOUTUPFACTOR);      }
"layoutnearfactor"	{
		 RETURN(LEXWORD_LAYOUTNEARFACTOR);    }
"layout_nearfactor"	{
		 RETURN(LEXWORD_LAYOUTNEARFACTOR);    }
"layoutsplinefactor"	{
		 RETURN(LEXWORD_LAYOUTSPLINEFACTOR);  }
"layout_splinefactor"	{
		 RETURN(LEXWORD_LAYOUTSPLINEFACTOR);  }
"leftjustify"		{
		 RETURN(LEXWORD_LEFT_JUSTIFY);        } 
"left_justify"		{
		 RETURN(LEXWORD_LEFT_JUSTIFY);        } 
"leftmargin"		{
		 RETURN(LEXWORD_LEFT_MARGIN);         } 
"left_margin"		{
		 RETURN(LEXWORD_LEFT_MARGIN);         } 
"leftneighbor"		{
		 RETURN(LEXWORD_LEFT_NEIGHBOR);       }
"left_neighbor"		{
		 RETURN(LEXWORD_LEFT_NEIGHBOR);       }
"lefttoright"		{
		 RETURN(LEXWORD_LEFT_TO_RIGHT);       } 
"left_to_right"		{
		 RETURN(LEXWORD_LEFT_TO_RIGHT);       } 
"left"			{
		 RETURN(LEXWORD_LEFT);                }
"level"			{
		 RETURN(LEXWORD_LEVEL);               }
"verticalorder"		{
		 RETURN(LEXWORD_VORDER);              }
"vertical_order"	{
		 RETURN(LEXWORD_VORDER);              }
"lightblue"		{
		 RETURN(LEXWORD_LIGHTBLUE);           }
"lightcyan"		{
		 RETURN(LEXWORD_LIGHTCYAN);           }
"lightgreen"		{
		 RETURN(LEXWORD_LIGHTGREEN);          }
"lightgray"		{
		 RETURN(LEXWORD_LIGHTGREY);           }
"lightgrey"		{
		 RETURN(LEXWORD_LIGHTGREY);           }
"lightmagenta"		{
		 RETURN(LEXWORD_LIGHTMAGENTA);        }
"lightred"		{
		 RETURN(LEXWORD_LIGHTRED);            }
"lightyellow"		{
		 RETURN(LEXWORD_LIGHTYELLOW);         }
"lilac"			{
		 RETURN(LEXWORD_LILAC);               }
"limit"			{
		 RETURN(LEXWORD_LIMIT);               } 
"line"			{
		 RETURN(LEXWORD_LINE);                } 
"line_style"		{
		 RETURN(LEXWORD_LINESTYLE);           } 
"linestyle"		{
		 RETURN(LEXWORD_LINESTYLE);           } 
"loc:"			{
		 RETURN(LEXWORD_LOC);                 }
"lowerneighbor"		{
		 RETURN(LEXWORD_LOWER_NEIGHBOR);      }
"lower_neighbor"	{
		 RETURN(LEXWORD_LOWER_NEIGHBOR);      }
"lowmargin"		{
		 RETURN(LEXWORD_LOW_MARGIN);          } 
"low_margin"		{
		 RETURN(LEXWORD_LOW_MARGIN);          } 
"low"			{
		 RETURN(LEXWORD_LOW);                 } 
"magenta"		{
		 RETURN(LEXWORD_MAGENTA);             }
"manhattenedges"	{
		 RETURN(LEXWORD_MANHATTEN);           } 
"manhattanedges"	{
		 RETURN(LEXWORD_MANHATTEN);           } 
"manhatten_edges"	{
		 RETURN(LEXWORD_MANHATTEN);           } 
"manhattan_edges"	{
		 RETURN(LEXWORD_MANHATTEN);           } 
"manual"		{
		 RETURN(LEXWORD_MANUAL);              } 
"maxdepthslow"		{
		 RETURN(LEXWORD_MAXDEPTHSLOW);        } 
"maxdepth"		{
		 RETURN(LEXWORD_MAXDEPTH);            } 
"maxdegree"		{
		 RETURN(LEXWORD_MAXDEGREE);           } 
"maxindegree"		{
		 RETURN(LEXWORD_MAXINDEGREE);         } 
"maxoutdegree"		{
		 RETURN(LEXWORD_MAXOUTDEGREE);        } 
"median"      	 	{
		 RETURN(LEXWORD_MEDIAN);              } 
"medianbary"   	 	{
		 RETURN(LEXWORD_MEDIANBARY);          } 
"mindepthslow"		{
		 RETURN(LEXWORD_MINDEPTHSLOW);        } 
"mindepth"		{
		 RETURN(LEXWORD_MINDEPTH);            } 
"mindegree"		{
		 RETURN(LEXWORD_MINDEGREE);           } 
"minindegree"		{
		 RETURN(LEXWORD_MININDEGREE);         } 
"minoutdegree"		{
		 RETURN(LEXWORD_MINOUTDEGREE);        } 
"minbackward"		{
		 RETURN(LEXWORD_MINBACK);             } 
"name"			{
		 RETURN(LEXWORD_NAME);                } 
"near_edge:"		{
		 RETURN(LEXWORD_NEAREDGE);            }
"nearedge:"		{
		 RETURN(LEXWORD_NEAREDGE);            }
"neighbors"		{
		 RETURN(LEXWORD_NEIGHBORS);           } 
"near_edges"		{
		 RETURN(LEXWORD_NEAREDGES);           }
"nearedges"		{
		 RETURN(LEXWORD_NEAREDGES);           }
"nonearedges"		{
		 RETURN(LEXWORD_NONEAREDGES);         }
"no_near_edges"		{
		 RETURN(LEXWORD_NONEAREDGES);         }
"no_nearedges"		{
		 RETURN(LEXWORD_NONEAREDGES);         }
"node."			{
		 RETURN(LEXWORD_NODE1);               }
"node:"			{
		 RETURN(LEXWORD_NODE2);               }
"nodes"			{
		 RETURN(LEXWORD_NODES);               }
"nodealignment"     	{
		 RETURN(LEXWORD_NODE_ALIGN);          }
"node_alignment"     	{
		 RETURN(LEXWORD_NODE_ALIGN);          }
"none"			{
		 RETURN(LEXWORD_NONE);                }
"no"			{
		 RETURN(LEXWORD_NO);                  }
"orange"		{
		 RETURN(LEXWORD_ORANGE);              }
"orchid"		{
		 RETURN(LEXWORD_ORCHID);              }
"orientation"		{
		 RETURN(LEXWORD_ORIENTATION);         } 
"output_function"	{
		 RETURN(LEXWORD_OUTPUTFUNCTION);      }
"outputfunction"	{
		 RETURN(LEXWORD_OUTPUTFUNCTION);      }
"pfish"			{
		 RETURN(LEXWORD_PFISH);               }
"pink"			{
		 RETURN(LEXWORD_PINK);                }
"planar"		{
		 RETURN(LEXWORD_PLANAR);              }
"pmin"			{
		 RETURN(LEXWORD_PMIN);                }
"pmax"			{
		 RETURN(LEXWORD_PMAX);                }
"portsharing"		{
		 RETURN(LEXWORD_PORTSHARING);         } 
"port_sharing"		{
		 RETURN(LEXWORD_PORTSHARING);         } 
"priorityphase"		{
		 RETURN(LEXWORD_PRIORITYPHASE);       }
"priority_phase"	{
		 RETURN(LEXWORD_PRIORITYPHASE);       }
"priority"		{
		 RETURN(LEXWORD_PRIORITY);            }
"purple"		{
		 RETURN(LEXWORD_PURPLE);              } 
"range"			{
		 RETURN(LEXWORD_RANGE);               } 
"red"			{
		 RETURN(LEXWORD_RED);                 }
"rhomb"			{
		 RETURN(LEXWORD_RHOMB);               }
"rightjustify"		{
		 RETURN(LEXWORD_RIGHT_JUSTIFY);       }
"right_justify"		{
		 RETURN(LEXWORD_RIGHT_JUSTIFY);       }
"rightmargin"		{
		 RETURN(LEXWORD_RIGHT_MARGIN);        }
"right_margin"		{
		 RETURN(LEXWORD_RIGHT_MARGIN);        }
"rightneighbor"		{
		 RETURN(LEXWORD_RIGHT_NEIGHBOR);      }
"right_neighbor"	{
		 RETURN(LEXWORD_RIGHT_NEIGHBOR);      }
"righttoleft"		{
		 RETURN(LEXWORD_RIGHT_TO_LEFT);       }
"right_to_left"		{
		 RETURN(LEXWORD_RIGHT_TO_LEFT);       }
"right"			{
		 RETURN(LEXWORD_RIGHT);               }
"rmin"			{
		 RETURN(LEXWORD_RMIN);                }
"rmax"			{
		 RETURN(LEXWORD_RMAX);                }
"scaling"		{
		 RETURN(LEXWORD_SCALING);             }
"shape"			{
		 RETURN(LEXWORD_SHAPE);               }
"shrink"		{
		 RETURN(LEXWORD_SHRINK);              }
"smax"			{
		 RETURN(LEXWORD_SMAX);                }
"smanhattenedges"	{
		 RETURN(LEXWORD_SMANHATTEN);          } 
"smanhattanedges"	{
		 RETURN(LEXWORD_SMANHATTEN);          } 
"smanhatten_edges"	{
		 RETURN(LEXWORD_SMANHATTEN);          } 
"smanhattan_edges"	{
		 RETURN(LEXWORD_SMANHATTEN);          } 
"size"			{
		 RETURN(LEXWORD_SIZE);                }
"smaller"		{
		 RETURN(LEXWORD_SMALLER);             }
"solid"			{
		 RETURN(LEXWORD_SOLID);               }
"source_name"		{
		 RETURN(LEXWORD_SOURCENAME);          }
"sourcename"		{
		 RETURN(LEXWORD_SOURCENAME);          }
"splines"		{
		 RETURN(LEXWORD_SPLINES);             }
"spline_factor"		{
		 RETURN(LEXWORD_LAYOUTSPLINEFACTOR);  }
"splinefactor"		{
		 RETURN(LEXWORD_LAYOUTSPLINEFACTOR);  }
"spread_level"		{
		 RETURN(LEXWORD_SPREADLEVEL);         }
"spreadlevel"		{
		 RETURN(LEXWORD_SPREADLEVEL);         }
"status"		{
		 RETURN(LEXWORD_STATUS);              }
"stretch"		{
		 RETURN(LEXWORD_STRETCH);             }
"straightphase"		{
		 RETURN(LEXWORD_STRAIGHTPHASE);       }
"straight_phase"	{
		 RETURN(LEXWORD_STRAIGHTPHASE);       }
"target_name"		{
		 RETURN(LEXWORD_TARGETNAME);          }
"targetname"		{
		 RETURN(LEXWORD_TARGETNAME);          }
"text_color"		{
		 RETURN(LEXWORD_TEXTCOLOR);           }
"textcolor"		{
		 RETURN(LEXWORD_TEXTCOLOR);           }
"text_mode"		{
		 RETURN(LEXWORD_TEXTMODE);            }
"textmode"		{
		 RETURN(LEXWORD_TEXTMODE);            }
"thickness"		{
		 RETURN(LEXWORD_THICKNESS);           }
"title"			{
		 RETURN(LEXWORD_TITLE);               } 
"top_sort"              {
		 RETURN(LEXWORD_TOPSORT);             }
"topsort"               {
		 RETURN(LEXWORD_TOPSORT);             }
"topmargin"		{
		 RETURN(LEXWORD_TOP_MARGIN);          }
"top_margin"		{
		 RETURN(LEXWORD_TOP_MARGIN);          }
"toptobottom"		{
		 RETURN(LEXWORD_TOP_TO_BOTTOM);       }
"top_to_bottom"		{
		 RETURN(LEXWORD_TOP_TO_BOTTOM);       }
"top"			{
		 RETURN(LEXWORD_TOP);                 } 
"tree"			{
		 RETURN(LEXWORD_TREE);                }
"tree_factor"		{
		 RETURN(LEXWORD_TREEFACTOR);          }
"treefactor"		{
		 RETURN(LEXWORD_TREEFACTOR);          }
"triangle"		{
		 RETURN(LEXWORD_TRIANGLE);            }
"turquoise"		{
		 RETURN(LEXWORD_TURQUOISE);           }
"type_name"		{
		 RETURN(LEXWORD_TYPENAME);            }
"typename"		{
		 RETURN(LEXWORD_TYPENAME);            }
"upperneighbor"		{
		 RETURN(LEXWORD_UPPER_NEIGHBOR);      }
"upper_neighbor"	{
		 RETURN(LEXWORD_UPPER_NEIGHBOR);      }
"view"			{
		 RETURN(LEXWORD_VIEW);                }
"white"			{
		 RETURN(LEXWORD_WHITE);               }
"width"			{
		 RETURN(LEXWORD_WIDTH);               }
"xbase"			{
		 RETURN(LEXWORD_XBASE);               }
"xmax"			{
		 RETURN(LEXWORD_XMAX);                }
"xraster"		{
		 RETURN(LEXWORD_XRASTER);             }
"xlraster"		{
		 RETURN(LEXWORD_XLRASTER);            }
"xscrollbar"		{
		 RETURN(LEXWORD_XSCROLLBAR);          }
"xspace"		{
		 RETURN(LEXWORD_XSPACE);              }
"xlspace"		{
		 RETURN(LEXWORD_XLSPACE);             }
"ybase"			{
		 RETURN(LEXWORD_YBASE);               }
"yellowgreen"		{
		 RETURN(LEXWORD_YELLOWGREEN);         }
"yellow"		{
		 RETURN(LEXWORD_YELLOW);              }
"yes"			{
		 RETURN(LEXWORD_YES);                 }
"ymax"			{
		 RETURN(LEXWORD_YMAX);                }
"yraster"		{
		 RETURN(LEXWORD_YRASTER);             }
"yscrollbar"		{
		 RETURN(LEXWORD_YSCROLLBAR);          }
"yspace"		{
		 RETURN(LEXWORD_YSPACE);              }

{LINEDIR1}              {
		 line_directive(yytext);       }
{LINEDIR2}              {
		 line_directive(yytext);       }
{PRAGMADIR}             {
		 /* ignore pragma directive */ }

[\f ]+			{
		 pos_nr+=strlen(yytext); }
[\t]			{
		 pos_nr++;               }
[\n]			{
		 line_nr++; pos_nr=1;    }

{INT}			{
		 yylval.lnum = atol(yytext); 
			  RETURN(LEX_INT); 
			}
{REAL}			{
		 yylval.lrealnum = atof(yytext); 
			  RETURN(LEX_FLOAT); 
			}
{CHAR}     	     	{
		 yylval.byte = yytext[1];
               		  RETURN(LEX_CHAR);
                	}
{STRING}                {
		 yytext[strlen(yytext)-1] = '\0';
			  escape_transl(&(yytext[1]));
	                  yylval.lnum = HashInsert(&(yytext[1]));
			  SKIPYYTEXT;
                          RETURN(LEX_STRING);
                        }
{COMM}			{
		 SKIPYYTEXT; }
{COMME}			{
		 SKIPYYTEXT; }

"{"			{
		 RETURN('{'); }
"}"			{
		 RETURN('}'); }
":"			{
		 RETURN(':'); }
"x"			{
		 RETURN('x'); }
"y"			{
		 RETURN('y'); }
"z"			{
		 RETURN('z'); }
"*"			{
		 RETURN('*'); }
"["			{
		 RETURN('['); }
"]"			{
		 RETURN(']'); }
"-"			{
		 RETURN('-'); }



.              { 
		 (void)sprintf(message,
			"scanner reads unexpected %c",yytext[0]);
		 yysyntaxerror(line_nr,pos_nr,message);
                 RETURN(' '); }

%%




/*--------------------------------------------------------------------*/
/*   Initialize scanner                                               */
/*--------------------------------------------------------------------*/

void init_lex(void)
{
	yy_init = 0;
}




/*--------------------------------------------------------------------*/
/*  Standard Hash Table Routines                                      */
/*--------------------------------------------------------------------*/
 
#ifdef HASHGENSTD 


#include <stdio.h>
#include <string.h>
#include <malloc.h>

/* Global Variables */
/*------------------*/

static char **decode = NULL;
long   table_size = 0L;

/*--------------------------------------------------------------------*/
/* Fatal error: exit with message                                     */
/*--------------------------------------------------------------------*/

static void fatal_error(char *message)
{
	(void)fprintf(stderr,"Fatal error: %s !\n",message);
	(void)fprintf(stderr,"Aborted !\n");
	exit(-1);
}


/*--------------------------------------------------------------------*/
/* Memory allocation for single strings                               */
/*--------------------------------------------------------------------*/

#ifndef ALIGN
#define ALIGN 8
#define IALIGN (ALIGN-1)
#endif

#ifndef STRINGBLOCKSIZE 
#define STRINGBLOCKSIZE 5000 
#endif 
 

#ifndef PARSEGENSTD

/* If we include already the standard parser library, this is not
 * necessary
 */


static char *parseheap = (char *)0;             /* the heap */ 
static char *parseheapstart;                    /* the base */
static char *parseheaptop;                      /* the top  */
static char *parseheapend;                      /* the end  */
static int  parseheapsize = STRINGBLOCKSIZE; /* the size of one block */


static void alloc_block(void)
{
        char * help, *help2;

	help =(char *)malloc(parseheapsize*sizeof(long));
        if (!help) fatal_error("memory exhausted");
	help2  = (char **)help;
	*help2 = (char *)parseheap;
	parseheap = help;
	parseheapstart = parseheaptop =
		(char *)((long)parseheap + (long)sizeof(char *));
	parseheapend = parseheap;
	parseheapend += (parseheapsize-2);
        if ((long)parseheaptop&IALIGN) 
		parseheaptop = (char *)
			(((long)parseheaptop+(long)IALIGN)&(long)(~IALIGN));
}


/*  allocate x bytes */

static char * ParseMalloc(int x)
{
        char * help;
        int  y;

	if (!parseheap) alloc_block();

        y = x;
        if (y&IALIGN) y = (y+IALIGN)&(~IALIGN);

        help = parseheaptop;
        parseheaptop = (char *)((long)parseheaptop+(long)y);

        if (parseheaptop > parseheapend) {

		/* heap too small -> allocate new heap block */

		alloc_block();
                help = parseheaptop;
                parseheaptop = (char *)((long)parseheaptop+(long)y);
        	if (parseheaptop > parseheapend) 
			fatal_error("string node too large");
        }
        return (help);
}


/* Global: allocate x bytes */

char *StringHeapMalloc(int x)
{
	return(ParseMalloc(x));
}


/* global deallocate the complete syntax tree heap */

void StringHeapFree(void)
{
        char *help, help2;

        help = parseheap;
        while (help) {
                help2 = *(char **)help;
                (void)free(help);
                help = help2;
        }

        parseheap = (char *)0;
}

#endif /* PARSEGENSTD */


/*--------------------------------------------------------------------*/
/* Memory allocation for decode table and hash table                  */
/*--------------------------------------------------------------------*/

#ifndef hash_size
#define hash_size 211
#endif
#ifndef hashtable_size
#define hashtable_size 10000L
#endif

/* We use a hash table with linked lists */

typedef struct hash_node {
        long              num;
        struct hash_node *next;
} *hashentry;


/* The hash table */

static hashentry *hashtable = NULL;
static long hashtop = 1;

static void init_hash(void)
{
	char *h;

        if (!hashtable) {

		/* no hash table present -> allocate table */

                hashtable  = (hashentry *) 
				calloc((size_t) (hash_size+1),
                				 sizeof(hashentry));
        	if (!hashtable) fatal_error("memory exhausted");
	}

        if (!decode) {

		/* no decode table present -> allocate table */

		hashtop = 1;
		table_size = hashtable_size;
                decode   = (char **) 
				calloc((size_t) (table_size+1L),
                   				sizeof(char *));
	        if (!decode) fatal_error("memory exhausted");
                h = ParseMalloc(strlen("$!+-#Start of Hash#+-!$")+1);
                decode[0]    = strcpy(h,"$!+-#Start of Hash#+-!$");
	}
}


static void increase_decode(void)
{
	char **help,**a,**b;
	long   mysize, j;

	mysize = table_size * 2L;
	help = (char **) calloc((size_t) (mysize+1), sizeof(char *));
	if (!help) fatal_error("memory exhausted");
 
	a = help;
	b = decode;
	for (j=0; j<table_size; j++) *a++ = *b++;
	if (decode) (void)free((char *)decode);
	
	decode   = help; 
	table_size = mysize;
}

void FreeHash(void)
{
	if (decode) (void)free((char *)decode);
	decode   = (char **)0; 
	if (hashtable) (void)free((char *)hashtable);
        hashtable  = (hashentry *)0;
}


/*--------------------------------------------------------------------*/
/* Hash table for identifers and strings                              */
/*--------------------------------------------------------------------*/


/* the hash function */

static int hash(register char *string)
{
        register int   hashval = 0 ;
        register char *s = string;

        for(hashval=0; *s != '\0';)
                hashval = (10*hashval - 48 + *s++) % hash_size;
        hashval = ((hashval<0 ) ? -hashval : hashval);

        return(hashval % hash_size);
        /* Modulo calculation because of table range */
}



/*--------------------------------------------------------------------*/
/* Hash table: visible from outside                                   */
/*--------------------------------------------------------------------*/

long HashInsert(register char *s)
{
        register  hashentry new_node;
        register  int       hashval;
        register  long      look;
        char *h;

        if (!hashtable) init_hash();

        hashval  = hash(s);
	new_node = hashtable[hashval];
	look = -1L;
        while (new_node != NULL) {
                if ( strcmp(s,decode[new_node->num]) == 0L) {
			look = new_node->num;
			break;
		}
                new_node=new_node->next;
        }

        if (look == -1L) {
                if (hashtop == table_size-1) increase_decode();
                new_node = (hashentry)ParseMalloc(sizeof(struct hash_node));
                look = hashtop++;
                new_node->num  = look;
                new_node->next = hashtable[hashval];

                h = ParseMalloc(strlen(s)+1);
                decode[look]    = strcpy(h,s);
                hashtable[hashval] = new_node;
        }
        return(look);
}

long HashTableSize(void)
{
        if (!hashtable) init_hash();
	return(hashtop);
}

char *Decode(long x)
{
        if (!hashtable) init_hash();
	if (x<0)       return(decode[0]);
	if (x<hashtop) return(decode[x]);
	return(decode[0]);
}


#endif /* HASHGENSTD */

/*--------------------------------------------------------------------*/
 



