/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "parser.y"


#define YYERROR_VERBOSE

#include <stdio.h>
#include <stdint.h>
#include "scanner.hpp"
#include "astnodes.hpp"
#include "dictionnary.hpp"

  Expression*	check_for_timestamp(Expression* lhs, Expression* rhs);


/* Line 268 of yacc.c  */
#line 160 "parser.y"


//Function declaration



/* Line 268 of yacc.c  */
#line 93 "/home/udgover/projects/dff_merge_mvc_tags/api/filters/parser.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     CONTAIN = 258,
     DATA = 259,
     DOTDOT = 260,
     ATTRIBUTE = 261,
     FILESIZE = 262,
     TIMESTAMP = 263,
     IDENTIFIER = 264,
     NUMBER = 265,
     STRING = 266,
     REGEXP = 267,
     WILDCARD = 268,
     FUZZY = 269,
     DICT = 270,
     ATTRIBUTES = 271,
     MATCHES = 272,
     _TRUE_ = 273,
     _FALSE_ = 274,
     ALL = 275,
     ANY = 276,
     NONE = 277,
     OF = 278,
     OR = 279,
     AND = 280,
     BIN_OR = 281,
     BIN_XOR = 282,
     BIN_AND = 283,
     LT = 284,
     LTE = 285,
     GT = 286,
     GTE = 287,
     EQ = 288,
     NEQ = 289,
     IS = 290,
     _IN_ = 291,
     LSHIFT = 292,
     RSHIFT = 293,
     PLUS = 294,
     MINUS = 295,
     MUL = 296,
     DIV = 297,
     MOD = 298,
     POW = 299,
     NOT = 300
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 179 "/home/udgover/projects/dff_merge_mvc_tags/api/filters/parser.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  28
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   110

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  19
/* YYNRULES -- Number of rules.  */
#define YYNRULES  56
/* YYNRULES -- Number of states.  */
#define YYNSTATES  95

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   300

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      46,    47,     2,     2,    50,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    48,     2,    49,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     9,    13,    16,    20,    22,    24,
      26,    30,    32,    36,    38,    42,    46,    48,    50,    56,
      62,    68,    70,    74,    78,    83,    87,    91,    93,    95,
      97,    99,   101,   103,   105,   109,   113,   117,   121,   125,
     129,   131,   133,   137,   139,   141,   143,   145,   147,   149,
     151,   153,   155,   157,   159,   164,   166
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      52,     0,    -1,    53,    -1,    53,    25,    53,    -1,    53,
      24,    53,    -1,    45,    53,    -1,    46,    53,    47,    -1,
      63,    -1,    60,    -1,    56,    -1,    48,    59,    49,    -1,
      15,    -1,    48,    55,    49,    -1,    15,    -1,    55,    50,
      15,    -1,    61,    36,    54,    -1,    57,    -1,    58,    -1,
       6,    17,    67,    23,    54,    -1,     9,    17,    67,    23,
      54,    -1,     4,    17,    67,    23,    54,    -1,    62,    -1,
      59,    50,    62,    -1,     6,    17,    62,    -1,     4,    17,
      10,    62,    -1,     4,    17,    62,    -1,     9,    17,    62,
      -1,     6,    -1,     9,    -1,    11,    -1,    12,    -1,    13,
      -1,    14,    -1,    68,    -1,    64,    29,    64,    -1,    64,
      30,    64,    -1,    64,    31,    64,    -1,    64,    32,    64,
      -1,    64,    33,    64,    -1,    64,    34,    64,    -1,    65,
      -1,    66,    -1,    46,    64,    47,    -1,    10,    -1,     6,
      -1,     9,    -1,    11,    -1,     8,    -1,    18,    -1,    19,
      -1,    10,    -1,    21,    -1,    20,    -1,    22,    -1,     9,
      46,    69,    47,    -1,    11,    -1,    69,    50,     9,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   171,   171,   178,   179,   180,   181,   182,   183,   184,
     188,   189,   208,   214,   233,   269,   270,   271,   274,   304,
     321,   350,   351,   371,   384,   393,   402,   421,   422,   434,
     449,   464,   479,   494,   535,   543,   551,   559,   567,   575,
     597,   628,   629,   633,   634,   635,   645,   646,   647,   648,
     662,   663,   664,   665,   668,   696,   713
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CONTAIN", "DATA", "DOTDOT", "ATTRIBUTE",
  "FILESIZE", "TIMESTAMP", "IDENTIFIER", "NUMBER", "STRING", "REGEXP",
  "WILDCARD", "FUZZY", "DICT", "ATTRIBUTES", "MATCHES", "_TRUE_",
  "_FALSE_", "ALL", "ANY", "NONE", "OF", "OR", "AND", "BIN_OR", "BIN_XOR",
  "BIN_AND", "LT", "LTE", "GT", "GTE", "EQ", "NEQ", "IS", "_IN_", "LSHIFT",
  "RSHIFT", "PLUS", "MINUS", "MUL", "DIV", "MOD", "POW", "NOT", "'('",
  "')'", "'['", "']'", "','", "$accept", "input", "boolean_expression",
  "pattern_container", "dict_list", "contain_expression",
  "contain_expression_attr", "contain_expression_id", "pattern_list",
  "match_expression", "attribute_id", "pattern", "comparison_expression",
  "expression", "primary", "literal", "counter", "search_function",
  "search_func_params", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,    40,    41,    91,    93,
      44
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    51,    52,    53,    53,    53,    53,    53,    53,    53,
      54,    54,    54,    55,    55,    56,    56,    56,    57,    58,
      58,    59,    59,    60,    60,    60,    60,    61,    61,    62,
      62,    62,    62,    62,    63,    63,    63,    63,    63,    63,
      64,    65,    65,    66,    66,    66,    66,    66,    66,    66,
      67,    67,    67,    67,    68,    69,    69
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     3,     3,     2,     3,     1,     1,     1,
       3,     1,     3,     1,     3,     3,     1,     1,     5,     5,
       5,     1,     3,     3,     4,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    44,    47,    45,    43,    46,    48,    49,     0,
       0,     0,     2,     9,    16,    17,     8,     0,     7,     0,
      40,    41,     0,     0,     0,     5,     0,     0,     1,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      29,    30,    31,    32,    52,    51,    53,    25,     0,    33,
      50,    23,     0,    26,     0,     6,    42,     4,     3,    11,
       0,    15,    44,    45,     0,    34,    35,    36,    37,    38,
      39,     0,    24,     0,     0,     0,    13,     0,     0,    21,
       0,    55,     0,    20,    18,    19,    12,     0,    10,     0,
      54,     0,    14,    22,    56
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    11,    12,    61,    77,    13,    14,    15,    78,    16,
      17,    47,    18,    19,    20,    21,    48,    49,    82
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -29
static const yytype_int8 yypact[] =
{
      -1,    -6,   -13,   -29,    -5,   -29,   -29,   -29,   -29,    -1,
      -1,    15,    38,   -29,   -29,   -29,   -29,     2,   -29,    70,
     -29,   -29,    62,    76,    76,   -29,   -11,    17,   -29,    -1,
      -1,    -9,    24,    24,    24,    24,    24,    24,    19,    96,
     -29,   -29,   -29,   -29,   -29,   -29,   -29,   -29,    58,   -29,
     -29,   -29,    68,   -29,    69,   -29,   -29,    44,   -29,   -29,
      46,   -29,   -29,   -29,    24,   -29,   -29,   -29,   -29,   -29,
     -29,    82,   -29,    -9,    -9,    -9,   -29,    18,    28,   -29,
      47,   -29,   -28,   -29,   -29,   -29,   -29,    80,   -29,    96,
     -29,    97,   -29,   -29,   -29
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -29,   -29,    11,   -21,   -29,   -29,   -29,   -29,   -29,   -29,
     -29,   -23,   -29,    -8,   -29,   -29,    56,   -29,   -29
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -29
static const yytype_int8 yytable[] =
{
      51,    53,    27,     1,    23,     2,    59,     3,     4,     5,
       6,    22,    24,    29,    30,    28,    72,     7,     8,    90,
      25,    26,    91,   -27,    65,    66,    67,    68,    69,    70,
      62,   -28,     3,    63,     5,     6,    55,    79,    31,    60,
      57,    58,     7,     8,     9,    10,    32,    33,    34,    35,
      36,    37,    83,    84,    85,    38,    80,    40,    41,    42,
      43,    76,    29,    30,    56,    71,    93,    86,    87,    30,
      64,    38,    39,    40,    41,    42,    43,    88,    89,    52,
      54,    73,    44,    45,    46,    38,    50,    40,    41,    42,
      43,    74,    75,    81,    56,    92,    44,    45,    46,    32,
      33,    34,    35,    36,    37,    38,    94,    40,    41,    42,
      43
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-29))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_uint8 yycheck[] =
{
      23,    24,    10,     4,    17,     6,    15,     8,     9,    10,
      11,    17,    17,    24,    25,     0,    39,    18,    19,    47,
       9,    10,    50,    36,    32,    33,    34,    35,    36,    37,
       6,    36,     8,     9,    10,    11,    47,    60,    36,    48,
      29,    30,    18,    19,    45,    46,    29,    30,    31,    32,
      33,    34,    73,    74,    75,     9,    64,    11,    12,    13,
      14,    15,    24,    25,    47,    46,    89,    49,    50,    25,
      46,     9,    10,    11,    12,    13,    14,    49,    50,    23,
      24,    23,    20,    21,    22,     9,    10,    11,    12,    13,
      14,    23,    23,    11,    47,    15,    20,    21,    22,    29,
      30,    31,    32,    33,    34,     9,     9,    11,    12,    13,
      14
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,     6,     8,     9,    10,    11,    18,    19,    45,
      46,    52,    53,    56,    57,    58,    60,    61,    63,    64,
      65,    66,    17,    17,    17,    53,    53,    64,     0,    24,
      25,    36,    29,    30,    31,    32,    33,    34,     9,    10,
      11,    12,    13,    14,    20,    21,    22,    62,    67,    68,
      10,    62,    67,    62,    67,    47,    47,    53,    53,    15,
      48,    54,     6,     9,    46,    64,    64,    64,    64,    64,
      64,    46,    62,    23,    23,    23,    15,    55,    59,    62,
      64,    11,    69,    54,    54,    54,    49,    50,    49,    50,
      47,    50,    15,    62,     9
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (yyscanner, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, yyscanner)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, yyscanner); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, void* yyscanner)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yyscanner)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    void* yyscanner;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yyscanner);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, void* yyscanner)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yyscanner)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    void* yyscanner;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yyscanner);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, void* yyscanner)
#else
static void
yy_reduce_print (yyvsp, yyrule, yyscanner)
    YYSTYPE *yyvsp;
    int yyrule;
    void* yyscanner;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       , yyscanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule, yyscanner); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, void* yyscanner)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yyscanner)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    void* yyscanner;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yyscanner);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void* yyscanner);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void* yyscanner)
#else
int
yyparse (yyscanner)
    void* yyscanner;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1806 of yacc.c  */
#line 172 "parser.y"
    { 
  filter_ctx*	ctx = yyget_extra(yyscanner);
  ctx->root = (yyvsp[(1) - (1)].node); 
}
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 178 "parser.y"
    { (yyval.node) = new LogicalAnd((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 179 "parser.y"
    { (yyval.node) = new LogicalOr((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 180 "parser.y"
    { (yyval.node) = new LogicalNot((yyvsp[(2) - (2)].node)); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 181 "parser.y"
    { (yyval.node) = (yyvsp[(2) - (3)].node); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 182 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 183 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 184 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 188 "parser.y"
    { (yyval.pcontainer) = (yyvsp[(2) - (3)].pcontainer); }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 190 "parser.y"
    {
  try
    {
      Dictionnary* dict = DictRegistry::instance()->get(*(yyvsp[(1) - (1)].cstr));
      delete (yyvsp[(1) - (1)].cstr);
      if (((yyval.pcontainer) = new PatternDictionnary(dict)) == NULL)
	{
	  yyerror(yyscanner, NULL);
	  YYERROR;
	}
    }
  catch (std::string)
    {
      delete (yyvsp[(1) - (1)].cstr);
      yyerror(yyscanner, NULL);
      YYERROR;
    }
}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 209 "parser.y"
    {
  (yyval.pcontainer) = (yyvsp[(2) - (3)].pcontainer);
}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 215 "parser.y"
    { 
  try
    {
      Dictionnary* dict = DictRegistry::instance()->get(*(yyvsp[(1) - (1)].cstr));
      delete (yyvsp[(1) - (1)].cstr);
      if (((yyval.pcontainer) = new PatternDictionnary(dict)) == NULL)
	{
	  yyerror(yyscanner, NULL);
	  YYERROR;
	}
    }
  catch (std::string)
    {
      delete (yyvsp[(1) - (1)].cstr);
      yyerror(yyscanner, NULL);
      YYERROR;
    }
}
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 234 "parser.y"
    {
  try
    {
      Dictionnary* dict = DictRegistry::instance()->get(*(yyvsp[(3) - (3)].cstr));
      delete (yyvsp[(3) - (3)].cstr);
      ((PatternDictionnary*)(yyvsp[(1) - (3)].pcontainer))->push(dict);
    }
  catch (std::string)
    {
      delete (yyvsp[(3) - (3)].cstr);
      yyerror(yyscanner, NULL);
      YYERROR;
    }
}
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 269 "parser.y"
    { (yyval.node) = new AttributeExpression((yyvsp[(1) - (3)].node), 1, (yyvsp[(3) - (3)].pcontainer)); }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 270 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 271 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 275 "parser.y"
    {
  Expression*	expr;
  
  if ((expr = new NamedAttribute(*(yyvsp[(1) - (5)].cstr))) != NULL)
    (yyval.node) = new AttributeExpression(expr, (yyvsp[(3) - (5)].number), (yyvsp[(5) - (5)].pcontainer));
  else
    {
      delete (yyvsp[(1) - (5)].cstr);
      delete (yyvsp[(5) - (5)].pcontainer);
      yyerror(yyscanner, NULL);
      YYERROR;
    }
}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 305 "parser.y"
    {

  Expression*	expr;
  
  if ((expr = AttributeFactory::instance()->create(*(yyvsp[(1) - (5)].cstr))) != NULL)
    {
      delete (yyvsp[(1) - (5)].cstr);
      (yyval.node) = new AttributeExpression(expr, (yyvsp[(3) - (5)].number), (yyvsp[(5) - (5)].pcontainer));
    }
  else
    {
      delete (yyvsp[(1) - (5)].cstr);
      yyerror(yyscanner, NULL);
      YYERROR;
    }
}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 322 "parser.y"
    {
  (yyval.node) = new DataPatternCount((yyvsp[(3) - (5)].number), (yyvsp[(5) - (5)].pcontainer));
}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 350 "parser.y"
    { (yyval.pcontainer) = new PatternList((yyvsp[(1) - (1)].search)); }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 351 "parser.y"
    { ((PatternList*)(yyvsp[(1) - (3)].pcontainer))->push((yyvsp[(3) - (3)].search)); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 372 "parser.y"
    {
  Expression*	expr;
  
  if ((expr = new NamedAttribute(*(yyvsp[(1) - (3)].cstr))) != NULL)
    (yyval.node) = new MatchExpression(expr, (yyvsp[(3) - (3)].search));
  else
    {
      delete (yyvsp[(3) - (3)].search);
      yyerror(yyscanner, NULL);
      YYERROR;
    }    
}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 385 "parser.y"
    {
  if (((yyval.node) = new DataMatches((yyvsp[(3) - (4)].number), (yyvsp[(4) - (4)].search))) == NULL)
    {
      delete (yyvsp[(4) - (4)].search);
      yyerror(yyscanner, NULL);
      YYERROR;
    }
}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 394 "parser.y"
    {
  if (((yyval.node) = new DataMatches(1, (yyvsp[(3) - (3)].search))) == NULL)
    {
      delete (yyvsp[(3) - (3)].search);
      yyerror(yyscanner, NULL);
      YYERROR;
    }
}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 403 "parser.y"
    {
  Expression*	expr;
  
  if ((expr = AttributeFactory::instance()->create(*(yyvsp[(1) - (3)].cstr))) != NULL)
    {
      delete (yyvsp[(1) - (3)].cstr);
      (yyval.node) = new MatchExpression(expr, (yyvsp[(3) - (3)].search));
    }
  else
    {
      delete (yyvsp[(1) - (3)].cstr);
      delete (yyvsp[(3) - (3)].search);
      yyerror(yyscanner, NULL);
      YYERROR;
    }
}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 421 "parser.y"
    { (yyval.node) = new NamedAttribute(*(yyvsp[(1) - (1)].cstr)); delete (yyvsp[(1) - (1)].cstr); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 423 "parser.y"
    {
  (yyval.node) = AttributeFactory::instance()->create(*(yyvsp[(1) - (1)].cstr));
  delete (yyvsp[(1) - (1)].cstr);
  if ((yyval.node) == NULL)
    {
      yyerror(yyscanner, NULL);
      YYERROR;
    }
}
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 435 "parser.y"
    { 
  Search* s = new Search(*(yyvsp[(1) - (1)].cstr), Search::CaseInsensitive, Search::Fixed);
  delete (yyvsp[(1) - (1)].cstr);
  try
    {
      s->compile();
    }
  catch (std::string)
    {
      yyerror(yyscanner, NULL);
      YYERROR;
    }
  (yyval.search) = s;
}
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 450 "parser.y"
    {
  Search* s = new Search(*(yyvsp[(1) - (1)].cstr), Search::CaseInsensitive, Search::Regexp);
  try
    {
      s->compile();
    }
  catch (std::string)
    {
      yyerror(yyscanner, NULL);
      YYERROR;
    }
  (yyval.search) = s;
  delete (yyvsp[(1) - (1)].cstr);
}
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 465 "parser.y"
    {
  Search* s = new Search(*(yyvsp[(1) - (1)].cstr), Search::CaseInsensitive, Search::Wildcard);
  try
    {
      s->compile();
    }
  catch (std::string)
    {
      yyerror(yyscanner, NULL);
      YYERROR;
    }
  (yyval.search) = s;
  delete (yyvsp[(1) - (1)].cstr);
}
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 480 "parser.y"
    {
  Search* s = new Search(*(yyvsp[(1) - (1)].cstr), Search::CaseInsensitive, Search::Fuzzy);
  try
    {
      s->compile();
    }
  catch (std::string)
    {
      yyerror(yyscanner, NULL);
      YYERROR;
    }
  (yyval.search) = s;
  delete (yyvsp[(1) - (1)].cstr);
}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 495 "parser.y"
    {
  try
    {
      (yyvsp[(1) - (1)].search)->compile();
    }
  catch (std::string)
    {
      yyerror(yyscanner, NULL);
      YYERROR;
    }
  (yyval.search) = (yyvsp[(1) - (1)].search);
}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 536 "parser.y"
    { 
  Expression*	num_to_ts;
  if ((num_to_ts = check_for_timestamp((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node))) != NULL)
    (yyval.node) = new ComparisonExpression((yyvsp[(1) - (3)].node), num_to_ts, LT);
  else
    (yyval.node) = new ComparisonExpression((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), LT);
}
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 544 "parser.y"
    { 
  Expression*	num_to_ts;
  if ((num_to_ts = check_for_timestamp((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node))) != NULL)
    (yyval.node) = new ComparisonExpression((yyvsp[(1) - (3)].node), num_to_ts, LTE);
  else
    (yyval.node) = new ComparisonExpression((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), LTE);
}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 552 "parser.y"
    { 
  Expression*	num_to_ts;
  if ((num_to_ts = check_for_timestamp((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node))) != NULL)
    (yyval.node) = new ComparisonExpression((yyvsp[(1) - (3)].node), num_to_ts, GT); 
  else
    (yyval.node) = new ComparisonExpression((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), GT); 
}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 560 "parser.y"
    { 
  Expression*	num_to_ts;
  if ((num_to_ts = check_for_timestamp((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node))) != NULL)
    (yyval.node) = new ComparisonExpression((yyvsp[(1) - (3)].node), num_to_ts, GTE); 
  else
    (yyval.node) = new ComparisonExpression((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), GTE); 
}
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 568 "parser.y"
    { 
  Expression*	num_to_ts;
  if ((num_to_ts = check_for_timestamp((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node))) != NULL)
    (yyval.node) = new ComparisonExpression((yyvsp[(1) - (3)].node), num_to_ts, EQ); 
  else
    (yyval.node) = new ComparisonExpression((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), EQ); 
}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 576 "parser.y"
    { 
  Expression*	num_to_ts;
  if ((num_to_ts = check_for_timestamp((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node))) != NULL)
    (yyval.node) = new ComparisonExpression((yyvsp[(1) - (3)].node), num_to_ts, NEQ); 
  else
    (yyval.node) = new ComparisonExpression((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), NEQ); 
}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 597 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 628 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 629 "parser.y"
    { (yyval.node) = (yyvsp[(2) - (3)].node); }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 633 "parser.y"
    { (yyval.node) = new Number((yyvsp[(1) - (1)].number)); }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 634 "parser.y"
    { (yyval.node) = new NamedAttribute(*(yyvsp[(1) - (1)].cstr)); delete (yyvsp[(1) - (1)].cstr); }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 636 "parser.y"
    {
  (yyval.node) = AttributeFactory::instance()->create(*(yyvsp[(1) - (1)].cstr));
  delete (yyvsp[(1) - (1)].cstr);
  if ((yyval.node) == NULL)
    {
      yyerror(yyscanner, NULL);
      YYERROR;
    }
}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 645 "parser.y"
    { (yyval.node) = new String(*(yyvsp[(1) - (1)].cstr)); delete (yyvsp[(1) - (1)].cstr); }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 646 "parser.y"
    { (yyval.node) = new Timestamp(*(yyvsp[(1) - (1)].cstr)); delete (yyvsp[(1) - (1)].cstr); }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 647 "parser.y"
    { (yyval.node) = new Boolean(true); }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 648 "parser.y"
    { (yyval.node) = new Boolean(false); }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 662 "parser.y"
    { (yyval.number) = (yyvsp[(1) - (1)].number); }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 663 "parser.y"
    { (yyval.number) = 1; }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 664 "parser.y"
    { (yyval.number) = UINT64_MAX; }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 665 "parser.y"
    { (yyval.number) = 0; }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 669 "parser.y"
    {
  Search*	s;
  std::string	pattern;
  Search::CaseSensitivity	cs;

  s = NULL;
  pattern = *((yyvsp[(3) - (4)].sfp)->pattern);
  cs = (yyvsp[(3) - (4)].sfp)->cs;
  delete (yyvsp[(3) - (4)].sfp)->pattern;
  free((yyvsp[(3) - (4)].sfp));
  if (*(yyvsp[(1) - (4)].cstr) == "re")
    s = new Search(pattern, cs, Search::Regexp);
  else if (*(yyvsp[(1) - (4)].cstr) == "w")
    s = new Search(pattern, cs, Search::Wildcard);
  else if (*(yyvsp[(1) - (4)].cstr) == "fz")
    s = new Search(pattern, cs, Search::Fuzzy);
  else if (*(yyvsp[(1) - (4)].cstr) == "f")
    s = new Search(pattern, cs, Search::Fixed);
  else
    {
      yyerror(yyscanner, NULL);
      YYERROR;
    }
  (yyval.search) = s;
}
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 697 "parser.y"
    {
  sfunc_params*	sfp;
  
  sfp = NULL;
  if ((sfp = (sfunc_params*)malloc(sizeof(sfunc_params))) == NULL)
    {
      yyerror(yyscanner, NULL);
      YYERROR;
    }
  else
    {
      sfp->pattern = (yyvsp[(1) - (1)].cstr);
      sfp->cs = Search::CaseSensitive;
    }
  (yyval.sfp) = sfp;
}
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 714 "parser.y"
    {
  if (*(yyvsp[(3) - (3)].cstr) != "i")
    {
      yyerror(yyscanner, NULL);
      YYERROR;      
    }
  else
    (yyvsp[(1) - (3)].sfp)->cs = Search::CaseInsensitive;
  (yyval.sfp) = (yyvsp[(1) - (3)].sfp);
}
    break;



/* Line 1806 of yacc.c  */
#line 2169 "/home/udgover/projects/dff_merge_mvc_tags/api/filters/parser.cpp"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (yyscanner, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yyscanner, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, yyscanner);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yyscanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (yyscanner, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, yyscanner);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yyscanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 753 "parser.y"
 

Expression*	check_for_timestamp(Expression* lhs, Expression* rhs)
{
  TimestampAttribute*	tsattr = dynamic_cast<TimestampAttribute*>(lhs);
  Number*		numattr = dynamic_cast<Number*>(rhs);
  Expression*		ts = NULL;

  if (tsattr != NULL && numattr != NULL)
    {
      Variant*	vptr;
      if ((vptr = numattr->evaluate()) != NULL)
	{
	  try
	    {
	      uint32_t	val = vptr->value<uint32_t>();
	      ts = new Timestamp(val);
	      delete numattr;
	    }
	  catch (std::string)
	    {	      
	    }
	}
    }
  return ts;
}

