/****************************************************************************
 *									    *
 *  FILE	: DIB.H						    *
 *									    *
 *  DESCRIPTION : Header/include file for DIB.C		    *
 *									    *
 ****************************************************************************/

/* Macro to restrict a given value to an upper or lower boundary value */
#define BOUND(x,min,max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

/* Macro to swap two values */
#define SWAP(x,y)   ((x)^=(y)^=(x)^=(y))

/* Macro to find the minimum of two values */
#define MIN(x,y) (((x) <= (y)) : x ? y)

#define MINBAND 	50     /* Minimum band size used by the program */
#define BANDINCREMENT	20     /* Decrement for band size while trying	*/
			       /* to determine optimum band size.	*/

/* Bit values for the DIB attributes flag (fFileOptions). Also used as control
 * IDs for the radiobuttons for DIB bitcount in the File/Open dialog
 */
#define F_1BPP	    DLGOPEN_1BPP
#define F_4BPP	    DLGOPEN_4BPP
#define F_8BPP	    DLGOPEN_8BPP
#define F_24BPP     DLGOPEN_24BPP

/* Bit values for the DIB attributes flag (fFileOptions), also used as control
 * IDs for the radiobuttons for DIB compression type in the File/Open dialog
 */
#define F_RLE4	    DLGOPEN_RLE4
#define F_RLE8	    DLGOPEN_RLE8
#define F_RGB	    DLGOPEN_RGB

/* flags for _lseek */
#define  SEEK_CUR 1
#define  SEEK_END 2
#define  SEEK_SET 0

/* struct to be passed in for the SETDIBSCALING printer escape */
typedef struct {
	short ScaleMode;
	short dx, dy;
} DIBPARAMS;

/* Menu IDs */

#define	IDM_UPDATECOL	1000
#define	IDM_ANIMATE0	1001
#define	IDM_ANIMATE5	5
#define	IDM_ANIMATE20	20
#define	IDM_ANIMATE50	50
#define	IDM_ANIMATE100	100
#define	IDM_ANIMATE200	200
#define IDM_ANIMATE201	32000

#define IDM_OPEN	1002
#define IDM_EXIT	1003
#define IDM_ABOUT	1004
#define	IDM_DIBSCREEN	1005
#define	IDM_NOUGLY	1006
#define	IDM_STEALCOL	1009
#define	IDM_PRINT	1010
#define IDM_COPY	1012
#define IDM_MEMORYDIB	1014
#define IDM_SAVE	1015
#define IDM_TRANSPARENT 1016

#define IDM_PASTEDIB    1020
#define IDM_PASTEDDB    1021
#define IDM_PASTEPAL    1022

#define IDI_APP         2000

#define ABOUTBOX        100

extern HWND hWndApp;		       /* The handle to the app. window     */

#define MAXREAD  32768		       /* Number of bytes to be read during */
				       /* each read operation.		    */

/* Header signatutes for various resources */
#define BFT_ICON   0x4349   /* 'IC' */
#define BFT_BITMAP 0x4d42   /* 'BM' */
#define BFT_CURSOR 0x5450   /* 'PT' */

/* macro to determine if resource is a DIB */
#define ISDIB(bft) ((bft) == BFT_BITMAP)

/* Macro to align given value to the closest DWORD (unsigned long ) */
#define ALIGNULONG(i)	((i+3)/4*4)

/* Macro to determine to round off the given value to the closest byte */
#define WIDTHBYTES(i)	((i+31)/32*4)

#define PALVERSION      0x300
#define MAXPALETTE	256	  /* max. # supported palette entries */

/********** THE FOLLOWING ARE USED IN DLGOPEN.C  ************************/

/* IDs for controls in the DlgOpen dialog */
#define DLGOPEN_EDIT		101
#define DLGOPEN_FILE_LISTBOX	102
#define DLGOPEN_DIR_LISTBOX	103
#define DLGOPEN_PATH		104
#define DLGOPEN_TEXT            105
#define DLGOPEN_FOLDOUT         106
#define DLGOPEN_BIG             107
#define DLGOPEN_SMALL           108

#define DLGOPEN_OPTION          0xF000
#define DLGOPEN_1BPP		0x0001
#define DLGOPEN_4BPP		0x0002
#define DLGOPEN_8BPP		0x0004
#define DLGOPEN_24BPP		0x0008
#define DLGOPEN_RLE4		0x0010
#define DLGOPEN_RLE8		0x0020
#define DLGOPEN_RGB		0x0040

#define DLGOPEN_OPTION8         0x0080

/*  flags:
 *     The LOWORD is the standard FileOpen() flags (OF_*)
 *     the HIWORD can be any of the following:
 */
#define OF_MUSTEXIST	0x00010000  /* file must exist if the user hits Ok    */
#define OF_NOSHOWSPEC	0x00020000  /* DO NOT Show search spec in the edit box*/
#define OF_SHOWSPEC	0x00000000  /* Show the search spec in the edit box   */
#define OF_SAVE 	0x00040000  /* Ok button will say "Save"	      */
#define OF_OPEN 	0x00080000  /* Ok button will say "Open"	      */
#define OF_NOOPTIONS	0x00100000  /* Disable the options fold out	      */

/* Attributes for DlgDirLst() */
#define ATTRFILELIST	0x0000	      /* include files only	     */
#define ATTRDIRLIST	0xC010	      /* directories and drives ONLY */
#define CBEXTMAX	6	      /* Number of bytes in "\*.txt" */


#define IDF(id)     ((id) & ~DLGOPEN_OPTION)  /* extracts flag from control ID */
#define FID(f)	    ((f)  |  DLGOPEN_OPTION)  /* extracts control ID from flag */


/***************** FUNCTION DECLARATIONS *******************/

DWORD lread(int fh, VOID *pv, DWORD ul);
DWORD lwrite(int fh, VOID *pv, DWORD ul);

/***********************************************************/

/***********************************************************/
/* Declarations of functions used in dib.c module	   */
/***********************************************************/

HANDLE	    OpenDIB (LPSTR szFile);
BOOL	    WriteDIB (LPSTR szFile,HANDLE hdib);
WORD	    PaletteSize (VOID * pv);
WORD	    DibNumColors (VOID * pv);
HPALETTE    CreateDibPalette (HANDLE hdib);
HPALETTE    CreateBIPalette (LPBITMAPINFOHEADER lpbi);
HANDLE	    DibFromBitmap (HBITMAP hbm, DWORD biStyle, WORD biBits, HPALETTE hpal);
HBITMAP	    BitmapFromDib (HANDLE hdib, HPALETTE hpal);
BOOL	    DibBlt (HDC hdc, int x0, int y0, int dx, int dy, HANDLE hdib, int x1, int y1, LONG rop);
BOOL	    StretchDibBlt (HDC hdc, int x0, int y0, int dx, int dy, HANDLE hdib, int x1, int y1, int dx1, int dy1, LONG rop);
BOOL	    DibInfo (HANDLE hdib,LPBITMAPINFOHEADER lpbi);
HANDLE	    ReadDibBitmapInfo (int fh);
BOOL	    DrawBitmap (HDC hdc, int x, int y, HBITMAP hbm, DWORD rop);

