/*--------------------------------------------------------------------*/
/*                 A flex scanner specification                       */
/*--------------------------------------------------------------------*/

/*%option yylineno*/
%option noyywrap

%{

#include <stdio.h>
#include <string.h>
#include "grammar.h"
#include "globals.h"



#define YY_USER_ACTION    lex_rule_match( yytext);

%}



/*--------------------------------------------------------------------*/
/*   Character classes                                                */
/*--------------------------------------------------------------------*/

LETTER          [a-zA-Z]
DIGIT           [0-9]
INT             (\-)?{DIGIT}+
REAL            {INT}\.{INT}([E](\+|\-)?{INT})?
CHAR            '([^\\\']|\\.)+'
STRQ            [^\\\"]|\\.
STRING          \"({STRQ}*)\"
COMM            \/\*[^\*]*\*+([^\*\/][^\*]*\*+)*\/
COMME           \/\/[^\n]*
WSPACE          [ \t\f]*
LINEDIR1        {WSPACE}#{WSPACE}"line"?{WSPACE}{INT}{WSPACE}{STRING}.*\n
LINEDIR2        {WSPACE}#{WSPACE}"line"?{WSPACE}{INT}[^\"]*\n
PRAGMADIR       {WSPACE}#{WSPACE}"pragma".*\n



/*--------------------------------------------------------------------*/
/*   Token rules                                                      */
/*--------------------------------------------------------------------*/

%%


"above"                 { return(LEXWORD_ABOVE);               }
"anchor_points"         { return(LEXWORD_ANCHORPOINTS);        }
"anchorpoints"          { return(LEXWORD_ANCHORPOINTS);        }
"anchor"                { return(LEXWORD_ANCHOR);              }
"aquamarine"            { return(LEXWORD_AQUAMARINE);          }
"around"                { return(LEXWORD_AROUND);              }
"arrow_mode"            { return(LEXWORD_ARROWMODE);           }
"arrowmode"             { return(LEXWORD_ARROWMODE);           }
"arrow_height"          { return(LEXWORD_ARROWHEIGHT);         }
"arrowheight"           { return(LEXWORD_ARROWHEIGHT);         }
"arrow_width"           { return(LEXWORD_ARROWWIDTH);          }
"arrowwidth"            { return(LEXWORD_ARROWWIDTH);          }
"arrow_color"           { return(LEXWORD_ARROWCOLOR);          }
"arrowcolor"            { return(LEXWORD_ARROWCOLOR);          }
"arrow_style"           { return(LEXWORD_ARROWSTYLE);          }
"arrowstyle"            { return(LEXWORD_ARROWSTYLE);          }
"arrow_size"            { return(LEXWORD_ARROWSIZE);           }
"arrowsize"             { return(LEXWORD_ARROWSIZE);           }
"back_arrow_color"      { return(LEXWORD_BARROWCOLOR);         }
"backarrow_color"       { return(LEXWORD_BARROWCOLOR);         }
"backarrowcolor"        { return(LEXWORD_BARROWCOLOR);         }
"back_arrow_style"      { return(LEXWORD_BARROWSTYLE);         }
"backarrow_style"       { return(LEXWORD_BARROWSTYLE);         }
"backarrowstyle"        { return(LEXWORD_BARROWSTYLE);         }
"back_arrow_size"       { return(LEXWORD_BARROWSIZE);          }
"backarrow_size"        { return(LEXWORD_BARROWSIZE);          }
"backarrowsize"         { return(LEXWORD_BARROWSIZE);          }
"back_edge:"            { return(LEXWORD_BACKEDGE);            }
"backedge:"             { return(LEXWORD_BACKEDGE);            }
"bary_center"           { return(LEXWORD_BARYCENTER);          }
"barycenter"            { return(LEXWORD_BARYCENTER);          }
"bary"                  { return(LEXWORD_BARY);                }
"barymedian"            { return(LEXWORD_BARYMEDIAN);          }
"behind"                { return(LEXWORD_BEHIND);              }
"below"                 { return(LEXWORD_BELOW);               }
"black"                 { return(LEXWORD_BLACK);               }
"blue"                  { return(LEXWORD_BLUE);                }
"bmax"                  { return(LEXWORD_BMAX);                }
"border_color"          { return(LEXWORD_BORDERCOLOR);         }
"bordercolor"           { return(LEXWORD_BORDERCOLOR);         }
"border_width"          { return(LEXWORD_BORDERWIDTH);         }
"borderwidth"           { return(LEXWORD_BORDERWIDTH);         }
"bottom_margin"         { return(LEXWORD_BOTTOM_MARGIN);       }
"bottommargin"          { return(LEXWORD_BOTTOM_MARGIN);       }
"bottomtotop"           { return(LEXWORD_BOTTOM_TO_TOP);       }
"bottom_to_top"         { return(LEXWORD_BOTTOM_TO_TOP);       }
"bottom"                { return(LEXWORD_BOTTOM);              }
"box"                   { return(LEXWORD_BOX);                 }
"bent_near_edge:"       { return(LEXWORD_BENTNEAREDGE);        }
"bent_nearedge:"        { return(LEXWORD_BENTNEAREDGE);        }
"bentnearedge:"         { return(LEXWORD_BENTNEAREDGE);        }
"center"                { return(LEXWORD_CENTER);              }
"cfish"                 { return(LEXWORD_CFISH);               }
"class_name"            { return(LEXWORD_CLASSNAME);           }
"classname"             { return(LEXWORD_CLASSNAME);           }
"class"                 { return(LEXWORD_CLASS);               }
"cluster"               { return(LEXWORD_CLUSTER);             }
"cmin"                  { return(LEXWORD_CMIN);                }
"cmax"                  { return(LEXWORD_CMAX);                }
"color_entry"           { return(LEXWORD_COLORENTRY);          }
"colorentry"            { return(LEXWORD_COLORENTRY);          }
"color"                 { return(LEXWORD_COLOR);               }
"constraints"           { return(LEXWORD_CONSTRAINTS);         }
"constraint:"           { return(LEXWORD_CONSTRAINT);          }
"continuous"            { return(LEXWORD_CONTINUOUS);          }
"crossing_weight"       { return(LEXWORD_CROSSING_WEIGHT);     }
"crossingweight"        { return(LEXWORD_CROSSING_WEIGHT);     }
"crossing_optimization" { return(LEXWORD_CROSSING_OPT);        }
"crossingoptimization"  { return(LEXWORD_CROSSING_OPT);        }
"crossing_phase2"       { return(LEXWORD_CROSSING_P2);         }
"crossingphase2"        { return(LEXWORD_CROSSING_P2);         }
"cyan"                  { return(LEXWORD_CYAN);                }
"darkblue"              { return(LEXWORD_DARKBLUE);            }
"darkcyan"              { return(LEXWORD_DARKCYAN);            }
"darkgreen"             { return(LEXWORD_DARKGREEN);           }
"darkgray"              { return(LEXWORD_DARKGREY);            }
"darkgrey"              { return(LEXWORD_DARKGREY);            }
"darkmagenta"           { return(LEXWORD_DARKMAGENTA);         }
"darkred"               { return(LEXWORD_DARKRED);             }
"darkyellow"            { return(LEXWORD_DARKYELLOW);          }
"dashed"                { return(LEXWORD_DASHED);              }
"depth_first_search"    { return(LEXWORD_DFS);                 }
"depthfirstsearch"      { return(LEXWORD_DFS);                 }
"dfs"                   { return(LEXWORD_DFS);                 }
"dimension"             { return(LEXWORD_DIMENSION);           }
"dirtyedgelabels"       { return(LEXWORD_DIRTY_EDGE_LABELS);   }
"dirty_edge_labels"     { return(LEXWORD_DIRTY_EDGE_LABELS);   }
"displayedgelabels"     { return(LEXWORD_DISPLAY_EDGE_LABELS); }
"display_edge_labels"   { return(LEXWORD_DISPLAY_EDGE_LABELS); }
"dotted"                { return(LEXWORD_DOTTED);              }
"edge."                 { return(LEXWORD_EDGE1);               }
"edge:"                 { return(LEXWORD_EDGE2);               }
"edges"                 { return(LEXWORD_EDGES);               }
"ellipse"               { return(LEXWORD_ELLIPSE);             }
"equalcolumn"           { return(LEXWORD_EQUAL_COLUMN);        }
"equal_column"          { return(LEXWORD_EQUAL_COLUMN);        }
"equalposition"         { return(LEXWORD_EQUAL_POSITION);      }
"equal_position"        { return(LEXWORD_EQUAL_POSITION);      }
"equalrow"              { return(LEXWORD_EQUAL_ROW);           }
"equal_row"             { return(LEXWORD_EQUAL_ROW);           }
"equal"                 { return(LEXWORD_EQUAL);               }
"every"                 { return(LEXWORD_EVERY);               }
"fcfish"                { return(LEXWORD_FCFISH);              }
"fpfish"                { return(LEXWORD_FPFISH);              }
"fixed"                 { return(LEXWORD_FIXED);               }
"free"                  { return(LEXWORD_FREE);                }
"fine_tuning"           { return(LEXWORD_FINETUNING);          }
"finetuning"            { return(LEXWORD_FINETUNING);          }
"fold_edge."            { return(LEXWORD_FOLDEDGE);            }
"foldedge."             { return(LEXWORD_FOLDEDGE);            }
"fold_node."            { return(LEXWORD_FOLDNODE);            }
"foldnode."             { return(LEXWORD_FOLDNODE);            }
"folding"               { return(LEXWORD_FOLDING);             }
"font_name"             { return(LEXWORD_FONTNAME);            }
"fontname"              { return(LEXWORD_FONTNAME);            }
"gold"                  { return(LEXWORD_GOLD);                }
"graph:"                { return(LEXWORD_GRAPH);               }
"greater"               { return(LEXWORD_GREATER);             }
"green"                 { return(LEXWORD_GREEN);               }
"gray"                  { return(LEXWORD_GREY);                }
"grey"                  { return(LEXWORD_GREY);                }
"height"                { return(LEXWORD_HEIGHT);              }
"hidesingles"           { return(LEXWORD_HIDESINGLES);         }
"hide_singles"          { return(LEXWORD_HIDESINGLES);         }
"highmargin"            { return(LEXWORD_HIGH_MARGIN);         }
"high_margin"           { return(LEXWORD_HIGH_MARGIN);         }
"high"                  { return(LEXWORD_HIGH);                }
"hidden"                { return(LEXWORD_HIDDEN);              }
"horizontalorder"       { return(LEXWORD_HORDER);              }
"horizontal_order"      { return(LEXWORD_HORDER);              }
"icon_file"             { return(LEXWORD_ICONFILE);            }
"iconfile"              { return(LEXWORD_ICONFILE);            }
"icon_height"           { return(LEXWORD_ICONHEIGHT);          }
"iconheight"            { return(LEXWORD_ICONHEIGHT);          }
"icon_style"            { return(LEXWORD_ICONSTYLE);           }
"iconstyle"             { return(LEXWORD_ICONSTYLE);           }
"icon_width"            { return(LEXWORD_ICONWIDTH);           }
"iconwidth"             { return(LEXWORD_ICONWIDTH);           }
"ignore_singles"        { return(LEXWORD_HIDESINGLES);         }
"ignoresingles"         { return(LEXWORD_HIDESINGLES);         }
"include"               { return(LEXWORD_INCLUDE);             }
"infoname"              { return(LEXWORD_INFONAME);            }
"info1"                 { return(LEXWORD_INFO1);               }
"info2"                 { return(LEXWORD_INFO2);               }
"info3"                 { return(LEXWORD_INFO3);               }
"input_function"        { return(LEXWORD_INPUTFUNCTION);       }
"inputfunction"         { return(LEXWORD_INPUTFUNCTION);       }
"interval"              { return(LEXWORD_INTERVAL);            }
"invisible"             { return(LEXWORD_INVISIBLE);           }
"in_front"              { return(LEXWORD_IN_FRONT);            }
"isi"                   { return(LEXWORD_ISI);                 }
"khaki"                 { return(LEXWORD_KHAKI);               }
"label_color"           { return(LEXWORD_TEXTCOLOR);           }
"labelcolor"            { return(LEXWORD_TEXTCOLOR);           }
"label"                 { return(LEXWORD_LABEL);               }
"lateedgelabels"        { return(LEXWORD_LATE_LABELS);         }
"late_edge_labels"      { return(LEXWORD_LATE_LABELS);         }
"layout_algorithm"      { return(LEXWORD_LAYOUTALGORITHM);     }
"layoutalgorithm"       { return(LEXWORD_LAYOUTALGORITHM);     }
"layout_frequency"      { return(LEXWORD_LAYOUTFREQUENCY);     }
"layoutfrequency"       { return(LEXWORD_LAYOUTFREQUENCY);     }
"layout_parameter"      { return(LEXWORD_LAYOUTPARAMETER);     }
"layoutparameter"       { return(LEXWORD_LAYOUTPARAMETER);     }
"layoutdownfactor"      { return(LEXWORD_LAYOUTDOWNFACTOR);    }
"layout_downfactor"     { return(LEXWORD_LAYOUTDOWNFACTOR);    }
"layoutupfactor"        { return(LEXWORD_LAYOUTUPFACTOR);      }
"layout_upfactor"       { return(LEXWORD_LAYOUTUPFACTOR);      }
"layoutnearfactor"      { return(LEXWORD_LAYOUTNEARFACTOR);    }
"layout_nearfactor"     { return(LEXWORD_LAYOUTNEARFACTOR);    }
"layoutsplinefactor"    { return(LEXWORD_LAYOUTSPLINEFACTOR);  }
"layout_splinefactor"   { return(LEXWORD_LAYOUTSPLINEFACTOR);  }
"leftjustify"           { return(LEXWORD_LEFT_JUSTIFY);        }
"left_justify"          { return(LEXWORD_LEFT_JUSTIFY);        }
"leftmargin"            { return(LEXWORD_LEFT_MARGIN);         }
"left_margin"           { return(LEXWORD_LEFT_MARGIN);         }
"leftneighbor"          { return(LEXWORD_LEFT_NEIGHBOR);       }
"left_neighbor"         { return(LEXWORD_LEFT_NEIGHBOR);       }
"lefttoright"           { return(LEXWORD_LEFT_TO_RIGHT);       }
"left_to_right"         { return(LEXWORD_LEFT_TO_RIGHT);       }
"left"                  { return(LEXWORD_LEFT);                }
"level"                 { return(LEXWORD_LEVEL);               }
"verticalorder"         { return(LEXWORD_VORDER);              }
"vertical_order"        { return(LEXWORD_VORDER);              }
"lightblue"             { return(LEXWORD_LIGHTBLUE);           }
"lightcyan"             { return(LEXWORD_LIGHTCYAN);           }
"lightgreen"            { return(LEXWORD_LIGHTGREEN);          }
"lightgray"             { return(LEXWORD_LIGHTGREY);           }
"lightgrey"             { return(LEXWORD_LIGHTGREY);           }
"lightmagenta"          { return(LEXWORD_LIGHTMAGENTA);        }
"lightred"              { return(LEXWORD_LIGHTRED);            }
"lightyellow"           { return(LEXWORD_LIGHTYELLOW);         }
"lilac"                 { return(LEXWORD_LILAC);               }
"limit"                 { return(LEXWORD_LIMIT);               }
"line"                  { return(LEXWORD_LINE);                }
"line_style"            { return(LEXWORD_LINESTYLE);           }
"linestyle"             { return(LEXWORD_LINESTYLE);           }
"loc:"                  { return(LEXWORD_LOC);                 }
"lowerneighbor"         { return(LEXWORD_LOWER_NEIGHBOR);      }
"lower_neighbor"        { return(LEXWORD_LOWER_NEIGHBOR);      }
"lowmargin"             { return(LEXWORD_LOW_MARGIN);          }
"low_margin"            { return(LEXWORD_LOW_MARGIN);          }
"low"                   { return(LEXWORD_LOW);                 }
"magenta"               { return(LEXWORD_MAGENTA);             }
"manhattenedges"        { return(LEXWORD_MANHATTEN);           }
"manhattanedges"        { return(LEXWORD_MANHATTEN);           }
"manhatten_edges"       { return(LEXWORD_MANHATTEN);           }
"manhattan_edges"       { return(LEXWORD_MANHATTEN);           }
"manual"                { return(LEXWORD_MANUAL);              }
"maxdepthslow"          { return(LEXWORD_MAXDEPTHSLOW);        }
"maxdepth"              { return(LEXWORD_MAXDEPTH);            }
"maxdegree"             { return(LEXWORD_MAXDEGREE);           }
"maxindegree"           { return(LEXWORD_MAXINDEGREE);         }
"maxoutdegree"          { return(LEXWORD_MAXOUTDEGREE);        }
"median"                { return(LEXWORD_MEDIAN);              }
"medianbary"            { return(LEXWORD_MEDIANBARY);          }
"mindepthslow"          { return(LEXWORD_MINDEPTHSLOW);        }
"mindepth"              { return(LEXWORD_MINDEPTH);            }
"mindegree"             { return(LEXWORD_MINDEGREE);           }
"minindegree"           { return(LEXWORD_MININDEGREE);         }
"minoutdegree"          { return(LEXWORD_MINOUTDEGREE);        }
"minbackward"           { return(LEXWORD_MINBACK);             }
"name"                  { return(LEXWORD_NAME);                }
"near_edge:"            { return(LEXWORD_NEAREDGE);            }
"nearedge:"             { return(LEXWORD_NEAREDGE);            }
"neighbors"             { return(LEXWORD_NEIGHBORS);           }
"near_edges"            { return(LEXWORD_NEAREDGES);           }
"nearedges"             { return(LEXWORD_NEAREDGES);           }
"nonearedges"           { return(LEXWORD_NONEAREDGES);         }
"no_near_edges"         { return(LEXWORD_NONEAREDGES);         }
"no_nearedges"          { return(LEXWORD_NONEAREDGES);         }
"node."                 { return(LEXWORD_NODE1);               }
"node:"                 { return(LEXWORD_NODE2);               }
"nodes"                 { return(LEXWORD_NODES);               }
"nodealignment"         { return(LEXWORD_NODE_ALIGN);          }
"node_alignment"        { return(LEXWORD_NODE_ALIGN);          }
"none"                  { return(LEXWORD_NONE);                }
"no"                    { return(LEXWORD_NO);                  }
"orange"                { return(LEXWORD_ORANGE);              }
"orchid"                { return(LEXWORD_ORCHID);              }
"orientation"           { return(LEXWORD_ORIENTATION);         }
"output_function"       { return(LEXWORD_OUTPUTFUNCTION);      }
"outputfunction"        { return(LEXWORD_OUTPUTFUNCTION);      }
"pfish"                 { return(LEXWORD_PFISH);               }
"pink"                  { return(LEXWORD_PINK);                }
"planar"                { return(LEXWORD_PLANAR);              }
"pmin"                  { return(LEXWORD_PMIN);                }
"pmax"                  { return(LEXWORD_PMAX);                }
"portsharing"           { return(LEXWORD_PORTSHARING);         }
"port_sharing"          { return(LEXWORD_PORTSHARING);         }
"priorityphase"         { return(LEXWORD_PRIORITYPHASE);       }
"priority_phase"        { return(LEXWORD_PRIORITYPHASE);       }
"priority"              { return(LEXWORD_PRIORITY);            }
"purple"                { return(LEXWORD_PURPLE);              }
"range"                 { return(LEXWORD_RANGE);               }
"red"                   { return(LEXWORD_RED);                 }
"rhomb"                 { return(LEXWORD_RHOMB);               }
"rightjustify"          { return(LEXWORD_RIGHT_JUSTIFY);       }
"right_justify"         { return(LEXWORD_RIGHT_JUSTIFY);       }
"rightmargin"           { return(LEXWORD_RIGHT_MARGIN);        }
"right_margin"          { return(LEXWORD_RIGHT_MARGIN);        }
"rightneighbor"         { return(LEXWORD_RIGHT_NEIGHBOR);      }
"right_neighbor"        { return(LEXWORD_RIGHT_NEIGHBOR);      }
"righttoleft"           { return(LEXWORD_RIGHT_TO_LEFT);       }
"right_to_left"         { return(LEXWORD_RIGHT_TO_LEFT);       }
"right"                 { return(LEXWORD_RIGHT);               }
"rmin"                  { return(LEXWORD_RMIN);                }
"rmax"                  { return(LEXWORD_RMAX);                }
"scaling"               { return(LEXWORD_SCALING);             }
"shape"                 { return(LEXWORD_SHAPE);               }
"shrink"                { return(LEXWORD_SHRINK);              }
"smax"                  { return(LEXWORD_SMAX);                }
"smanhattenedges"       { return(LEXWORD_SMANHATTEN);          }
"smanhattanedges"       { return(LEXWORD_SMANHATTEN);          }
"smanhatten_edges"      { return(LEXWORD_SMANHATTEN);          }
"smanhattan_edges"      { return(LEXWORD_SMANHATTEN);          }
"size"                  { return(LEXWORD_SIZE);                }
"smaller"               { return(LEXWORD_SMALLER);             }
"solid"                 { return(LEXWORD_SOLID);               }
"source_name"           { return(LEXWORD_SOURCENAME);          }
"sourcename"            { return(LEXWORD_SOURCENAME);          }
"splines"               { return(LEXWORD_SPLINES);             }
"spline_factor"         { return(LEXWORD_LAYOUTSPLINEFACTOR);  }
"splinefactor"          { return(LEXWORD_LAYOUTSPLINEFACTOR);  }
"spread_level"          { return(LEXWORD_SPREADLEVEL);         }
"spreadlevel"           { return(LEXWORD_SPREADLEVEL);         }
"status"                { return(LEXWORD_STATUS);              }
"stretch"               { return(LEXWORD_STRETCH);             }
"straightphase"         { return(LEXWORD_STRAIGHTPHASE);       }
"straight_phase"        { return(LEXWORD_STRAIGHTPHASE);       }
"target_name"           { return(LEXWORD_TARGETNAME);          }
"targetname"            { return(LEXWORD_TARGETNAME);          }
"text_color"            { return(LEXWORD_TEXTCOLOR);           }
"textcolor"             { return(LEXWORD_TEXTCOLOR);           }
"text_mode"             { return(LEXWORD_TEXTMODE);            }
"textmode"              { return(LEXWORD_TEXTMODE);            }
"thickness"             { return(LEXWORD_THICKNESS);           }
"title"                 { return(LEXWORD_TITLE);               }
"top_sort"              { return(LEXWORD_TOPSORT);             }
"topsort"               { return(LEXWORD_TOPSORT);             }
"topmargin"             { return(LEXWORD_TOP_MARGIN);          }
"top_margin"            { return(LEXWORD_TOP_MARGIN);          }
"toptobottom"           { return(LEXWORD_TOP_TO_BOTTOM);       }
"top_to_bottom"         { return(LEXWORD_TOP_TO_BOTTOM);       }
"top"                   { return(LEXWORD_TOP);                 }
"tree"                  { return(LEXWORD_TREE);                }
"tree_factor"           { return(LEXWORD_TREEFACTOR);          }
"treefactor"            { return(LEXWORD_TREEFACTOR);          }
"triangle"              { return(LEXWORD_TRIANGLE);            }
"turquoise"             { return(LEXWORD_TURQUOISE);           }
"type_name"             { return(LEXWORD_TYPENAME);            }
"typename"              { return(LEXWORD_TYPENAME);            }
"upperneighbor"         { return(LEXWORD_UPPER_NEIGHBOR);      }
"upper_neighbor"        { return(LEXWORD_UPPER_NEIGHBOR);      }
"view"                  { return(LEXWORD_VIEW);                }
"white"                 { return(LEXWORD_WHITE);               }
"width"                 { return(LEXWORD_WIDTH);               }
"xbase"                 { return(LEXWORD_XBASE);               }
"xmax"                  { return(LEXWORD_XMAX);                }
"xraster"               { return(LEXWORD_XRASTER);             }
"xlraster"              { return(LEXWORD_XLRASTER);            }
"xscrollbar"            { return(LEXWORD_XSCROLLBAR);          }
"xspace"                { return(LEXWORD_XSPACE);              }
"xlspace"               { return(LEXWORD_XLSPACE);             }
"ybase"                 { return(LEXWORD_YBASE);               }
"yellowgreen"           { return(LEXWORD_YELLOWGREEN);         }
"yellow"                { return(LEXWORD_YELLOW);              }
"yes"                   { return(LEXWORD_YES);                 }
"ymax"                  { return(LEXWORD_YMAX);                }
"yraster"               { return(LEXWORD_YRASTER);             }
"yscrollbar"            { return(LEXWORD_YSCROLLBAR);          }
"yspace"                { return(LEXWORD_YSPACE);              }

{LINEDIR1}              { line_directive(yytext);       }
{LINEDIR2}              { line_directive(yytext);       }
{PRAGMADIR}             { /* ignore pragma directive */ }

[\f ]+                  { }
[\t]                    { }
[\n]                    { }

{INT}                   {
                                yylval.lnum = atol(yytext);
                                return(LEX_INT);
                        }
{REAL}                  {
                                yylval.lrealnum = atof(yytext);
                                return(LEX_FLOAT);
                        }
{CHAR}                  {
                                yylval.byte = yytext[1];
                                return(LEX_CHAR);
                        }
{STRING}                {
                                yytext[strlen(yytext)-1] = '\0';
                                escape_transl(&(yytext[1]));
                                yylval.lnum = HashInsert(&(yytext[1]));
                                return(LEX_STRING);
                        }
{COMM}                  { }
{COMME}                 { }

"{"                     { return('{'); }
"}"                     { return('}'); }
":"                     { return(':'); }
"x"                     { return('x'); }
"y"                     { return('y'); }
"z"                     { return('z'); }
"*"                     { return('*'); }
"["                     { return('['); }
"]"                     { return(']'); }
"-"                     { return('-'); }

.                       {
                                syntaxerror(yylloc.first_line, yylloc.first_column,
                                        "scanner reads unexpected %c",yytext[0]);
                                return(' ');
                        }

%%




/*--------------------------------------------------------------------*/
/*   Initialize scanner                                               */
/*--------------------------------------------------------------------*/

void init_lex( FILE *f, const char *fname)
{
        yyin = f;
        line_nr = 1;
        pos_nr = 1;
        strncpy( filename, fname, FILENAME_BUF_SIZE-1);

        /* Turn yy_flex_debug on if you want to debug the scanner */
        yy_flex_debug = 0;

        /*yy_init = 0;*/
} /* init_lex */


