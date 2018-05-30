
/*
| Copyright (c) 2001, 2016, Oracle and/or its affiliates. All rights reserved.
|
| This file contains material copyrighted by Oracle and is the
| confidential unpublished property of Oracle, and may not be
| copied in whole or part, or disclosed to any third party, without the
| written prior consent of Oracle.
*/


/**
    \file   CASample.c

    \brief  A sample application for demonstrating consumption of the
            Outside In Content Access API.

    CASample provides an example of typical usage of the Outside In
    Content Access API (CA).  Because this is intended as a simple
    template or reference for common CA usage, it creates only
    rudimentary output.  However it does initialize, exercise and
    cleanup CA.

    Because using CA additionally requires the usage of the Outside In
    Data Access Module (DA), this application also demonstrates usage of
    a portion of DA.  The functions used from both CA and DA are briefly
    explained in this source file describing parameters given to each
    function and expected results including interpretation of the output
    and handling of common error conditions.  More thorough explanations
    are available in the documentation.
*/

/*
    This source file contains the following functions:

        useCA:          Process the given document through CA.
        handleBeginTag: Process SCCCA_BEGINTAG CA items.
        handleEndTag:   Process SCCCA_ENDTAG CA items.
        handleObject:   Process SCCCA_OBJECT CA items.
        handleStyle:    Process SCCCA_STYLECHANGE CA items.

        showUsage():    Show usage notes.
        displayError:   Display an error message.
        printCABuffer:  Print out a buffer.
        setOptions:     Set options related to CA.
*/


/*
    scctype.h provides general Outside In data types and is typically
    included in all code using Outside In technologies.  sccca.h includes
    datatypes, declarations, and constants specific to CA.
*/
#include <stdio.h>
#include <string.h>

#include "scctype.h"
#include "sccca.h"

/*
   Oracle does version tracking of the Outside In sample applications.
   OIT_VERSTRING is defined by internal build scripts so you can delete
   the following line at your leisure as it performs no real function.
*/
#ifdef OIT_VERSTRING
char oitsamplever[32] = OIT_VERSTRING;
#endif

/*
    ---------------------------------------------------------------------------
    Constants and declarations.
    ---------------------------------------------------------------------------
*/
/**
    \brief  The size of a buffer holding error description output.

    The descriptive text of the error will be truncated if needed.
*/
#define ERROR_BUFFER_SIZE 0x100
/**
    \brief  The size of the buffer to be filled with CA textual content.

    This buffer must be at least 1K bytes and should be 2 byte aligned.
*/
#define CONTENT_BUFFER_SIZE  0x1000

/**
    \brief  The size of the buffer to be filled with a tag name.

*/
#define CONTENT_NAME_SIZE  0x20

/**
    \brief Used with DAOpenDocument to indicate 
           how a document is being specified.
*/
#ifdef UNIX
#define PATH_TYPE   IOTYPE_UNIXPATH
#else
#define PATH_TYPE   IOTYPE_ANSIPATH
#endif


/**  \brief Process the given document through CA.  */
DAERR useCA(VTHDOC const hDoc, FILE* outputFile);
/**  \brief Process SCCCA_BEGINTAG CA items.  */
VTVOID handleBeginTag(FILE* outputFile, SCCCAGETCONTENT const * const caContent);
/**  \brief Process SCCCA_ENDTAG CA items.  */
VTVOID handleEndTag(FILE* outputFile, VTHDOC const hDoc, SCCCAGETCONTENT const * const caContent);
/**  \brief Process SCCCA_OBJECT CA items.  */
VTVOID handleObject(FILE* outputFile, VTHDOC const hDoc, SCCCAGETCONTENT const * const caContent);
/**  \brief Process SCCCA_STYLECHANGE CA items.  */
VTVOID handleStyle(FILE* outputFile, SCCCAGETCONTENT const * const caContent, VTBOOL const bBigEndian);

/**  \brief Show usage notes.  */
VTVOID showUsage();
/**  \brief Display an error message.  */
VTVOID displayError(DAERR const daErr, VTCHAR const * const source);
/**  \brief Print out a buffer.  */
VTVOID printCABuffer(FILE* outputFile, VTBYTE const * const buffer, VTDWORD const size, VTBOOL const bBigEndian);
/**  \brief Set options related to CA.  */
DAERR setOptions(FILE* outputFile, VTHDOC const hDoc);
/**  \brief Retrieve tag name  */
VTVOID retrieveTagName(VTDWORD const dwTag, VTCHAR * name);

/*
    ---------------------------------------------------------------------------
    Support functions.
    ---------------------------------------------------------------------------
*/
/**
    This function displays the help associated with this sample application.
*/
VTVOID showUsage()
{
    printf("CASample usage.\n");
    printf("casample input_file [-u outputfile]\n");
    printf("\tinput_file : The file to be consumed by CA.\n");
	printf("\tThe -u option will produce UTF-8 encoded unicode in outputfile\n");
}


/**
    The is the entry point for the application.  This program accepts an
    input file to be processed by CA on the command line, and writes
    output to stdout and errors to stderr.

    \param  argc    Number of arguments including the name of the
                    application.
    \param  argv    Array of pointers to characters representing command
                    line arguments.

    \return The exit status of the application.  0 for success.
*/
int main(int argc, char* argv[])
{
    VTHDOC hDoc;
    DAERR daErr;
    VTDWORD optionValue;
	VTDWORD dwXLSChanges;
	FILE* outputFile = stdout;	/* by default, write to stdout */

    /*  We must be given an input file or an input file -u and an output file.  */
    if (!((argc == 2) || (argc == 4)))
    {
        showUsage();
        return 1;
    }

	/* if the -u flag is set, open up the output file */
	if(argc == 4)
	{
		if((argv[2][0] == '-') && ((argv[2][1] == 'u') || (argv[2][1] == 'U')))
		{
			outputFile = fopen( argv[3], "w" );
			if( outputFile == NULL )
			{
				fprintf( stderr, "ERROR: Unable to open output file %s\n", argv[3] );
				return 1;
			}
			/* output a Byte Order Mark 0xfeff.  UTF-8 encoded yields 0xefbbbf */
			fprintf(outputFile, "%c", (VTBYTE)0xef);
			fprintf(outputFile, "%c", (VTBYTE)0xbb);
			fprintf(outputFile, "%c", (VTBYTE)0xbf);
		}
		else
		{
			 fprintf(stderr, "ERROR: Invalid flag in the second argument");
			 showUsage();
			 return 1;
		}
	}
    
    /*
        Initialize the Data Access module.  DAInitEx should be called once
        before any conversions are attempted.  DAInitEx does not need to be
        called for each individual conversion, but instead should be called
        once for a set of conversions.

        Successful calls to DAInitEx should be matched by calls to DADeInit.

        DAInitEx returns a DAERR code.  DAERR_OK indicates success.
    */
    daErr = DAInitEx(SCCOPT_INIT_NOTHREADS, OI_INIT_DEFAULT);
    if (daErr != DAERR_OK)
    {
        displayError(daErr, "DAInitEx");
        return daErr;
    }

    /*
        Open our input document.
        
        DAOpenDocument can attempt to identify the type of the input file
        so before calling it for the first time we set the file
        identification option to extended testing.

        DASetOption Parameters:
            1)  A handle to the document for which the option should be set.
                We have no document handle yet, so we use 0 to indicate that
                this option should be set for all documents.
            2)  The id of the option to set.
            3)  A pointer to the value the option will be set to.
            4)  The size of the option's value.

        DASetOption returns a DAERR code.  DAERR_OK indicates success.
    */
    optionValue = SCCUT_FI_EXTENDEDTEST;
    daErr = DASetOption(0, SCCOPT_FIFLAGS, &optionValue, sizeof(optionValue));
    if (daErr != DAERR_OK)
    {
        displayError(daErr, "DASetOption");
        DADeInit();
        return daErr;
    }


    /*
        DAOpenDocument creates a handle to a document suitable for use in
        various Outside In API calls.  Successful calls to DAOpenDocument
        should be matched by calls to DACloseDocument.

        DAOpenDocument Parameters:
            1)  The address of a document handle that will contain the
                created document handle when the call returns successfully.
            2)  Type indicating what information is being provided to open
                the document, in this case a file path. (The preprocessor
                symbol PATH_TYPE was defined as either IOTYPE_ANSIPATH or 
                IOTYPE_UNIXPATH depending on the platform.)
            3)  A pointer to information needed to open the document (a
                pointer to the path of the file in this case).
            4)  Flags.  The 0 for flags forces DA to identify the file type
                for us.
            
        DAOpenDocument returns a DAERR code.  DAERR_OK indicates success.
    */
    daErr = DAOpenDocument(&hDoc, PATH_TYPE, argv[1], 0);
    if (daErr != DAERR_OK)
    {
        displayError(daErr, "DAOpenDocument");
        DADeInit();
        return daErr;
    }

	/*
		Revision data isn't output by default and must be turned on using
		SCCOPT_GENERATEEXCELREVISIONS.  
	*/
	dwXLSChanges = (VTDWORD)1;
	daErr = DASetOption(hDoc, SCCOPT_GENERATEEXCELREVISIONS, &dwXLSChanges, sizeof(dwXLSChanges));
	    if (daErr != DAERR_OK)
    {
        displayError(daErr, "DASetOption");
        DADeInit();
        return daErr;
    }

    /*  Use CA to process the document.  */
    daErr = useCA(hDoc, outputFile);
    if (daErr != DAERR_OK)
    {
        displayError(daErr, "useCA");
        DACloseDocument(hDoc);
        DADeInit();
        return daErr;
    }

    /*
        Close the document.

        DACloseDocument Parameters:
            1)  The handle of the document to be closed.

        DACloseDocument returns a DAERR code.  DAERR_OK indicates success.
    */
    daErr = DACloseDocument(hDoc);
    if (daErr != DAERR_OK)
    {
        displayError(daErr, "DACloseDocument");
        DADeInit();
        return daErr;
    }

    /*
        Deinitialize DA.

        DADeInit returns a DAERR code.  DAERR_OK indicates success.
    */
    daErr = DADeInit();
    if (daErr != DAERR_OK)
    {
        displayError(daErr, "DADeInit");
    }

    return daErr;
}


/**
    This function sets document specific options of interest to CA.
    Specifically processing of generated text is enabled, the default input
    character set is set to UTF8, the output character set is set to ANSI1252,
    and XMP metadata is disabled.  Any errors generated while setting options
    are reported, but processing is allowed to continue.

	\param	outputFile	A pointer to stdout or the output file.
    \param  hDoc    A handle to the document for which options are being set.

    \return A DA related error code.  DAERR_OK for success.
*/
#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

DAERR setOptions(FILE* outputFile, VTHDOC const hDoc)
{
    DAERR daErr;
    VTDWORD dwordOptionValue;
    VTBOOL  bExtractXMPMetadata = FALSE;

    /*  Turn on processing of generated text.  */
/*
        DASetOption Parameters:
            1)  A handle to the document for which the option should be set.
            2)  The id of the option to set
            3)  A pointer to the value the option will be set to.
            4)  The size of the option's value.

        DASetOption returns a DAERR code.  DAERR_OK indicates success.
*/
    dwordOptionValue = SCCEX_IND_GENERATED;
    daErr = DASetOption(hDoc, SCCOPT_CA_FLAGS, &dwordOptionValue, sizeof(dwordOptionValue));
    if (daErr != DAERR_OK)
    {
        displayError(daErr, "DASetOption");
    }

    /*  Set the fallback format to the FI_NONE */
    dwordOptionValue = FI_NONE;
    daErr = DASetOption(hDoc, SCCOPT_FALLBACKFORMAT, &dwordOptionValue, sizeof(dwordOptionValue));
    if (daErr != DAERR_OK)
    {
        displayError(daErr, "DASetOption");
    }

    /*  Set default input character set.  */
    dwordOptionValue = SO_ANSI1252;
    daErr = DASetOption(hDoc, SCCOPT_DEFAULTINPUTCHARSET, &dwordOptionValue, sizeof(dwordOptionValue));
    if (daErr != DAERR_OK)
    {
        displayError(daErr, "DASetOption");
    }

    /*  Set output character set.  */
	if(outputFile == stdout)
		dwordOptionValue = SO_ANSI1252;
	else
		dwordOptionValue = SO_UNICODE; 
    daErr = DASetOption(hDoc, SCCOPT_OUTPUTCHARACTERSET, &dwordOptionValue, sizeof(dwordOptionValue));
    if (daErr != DAERR_OK)
    {
        displayError(daErr, "DASetOption");
    }

    /* set extract xmp metadata */
    daErr = DASetOption(hDoc, SCCOPT_EXTRACTXMPMETADATA, &bExtractXMPMetadata, sizeof(VTBOOL));
    if (daErr != DAERR_OK)
    {
        displayError(daErr, "DASetOption");
    }
    return daErr;
}


/**
    This function prints out the given number of bytes from the given buffer
    to stdout.  Textual buffers related to CA can contain internal nulls so
    we cannot depend on the buffer being null terminated.  A leading tab and
    trailing new line are added.
	If the outputFile is set to stdout, then UTF-8 encoded the characters

	\param	outputFile	A pointer to stdout or the output file.
    \param  buffer  Pointer to a buffer to be printed.
    \param  size    The number of bytes to be printed.
*/
VTVOID printCABuffer(FILE* outputFile, VTBYTE const * const buffer, VTDWORD const size, VTBOOL const bBigEndian)
{
    VTDWORD i;
	VTBYTE tempChar;

	fprintf(outputFile, "\t");
	if(outputFile == stdout)
	{
		/* just output each character, since they will all be single byte characters */
		for (i = 0; i < size; i++)
		{
			fprintf(outputFile, "%c", buffer[i]);
		}
	}
	else
	{
		/* This is a Unicode Byte string.  Each character will be checked to see if it 
		   needs to be UTF-8 encoded.  Note, the index is incremented by two each time
		   because each Unicode character requires two bytes */
		for (i = 0; i < size; i+=2)
		{
			/* create the full Unicode character */
      VTWORD wChar = 0;
      if (bBigEndian)
      {
          wChar = (buffer[i] << 8) | buffer[i + 1];
      }
      else
      {
          wChar = (buffer[i + 1] << 8) | buffer[i];
      }

			/* Characters below 0x007f are just output */
			if(wChar <= 0x007f)
			{
				fprintf(outputFile, "%c", wChar);
			}
			else
			{
				/* Characters between 0x007f and 0x07ff take two bytes */
				if(wChar <= 0x07ff)
				{
					tempChar =  0xc0 | (VTBYTE)(wChar >> 6);
					fprintf(outputFile, "%c", tempChar);
					tempChar =  0x80 | (VTBYTE)(wChar & 0x3f);
					fprintf(outputFile, "%c", tempChar);
				}
				else
				{
					/* Characters above 0x07ff require three bytes */
					tempChar =  0xe0 | (VTBYTE)(wChar >> 12);
					fprintf(outputFile, "%c", tempChar);
					tempChar =  0x80 | (VTBYTE)((wChar >> 6) & 0x3f);
					fprintf(outputFile, "%c", tempChar);
					tempChar =  0x80 | (VTBYTE)(wChar & 0x3f);
					fprintf(outputFile, "%c", tempChar);
				}
			}
		}
	}
    fprintf(outputFile, "\n");
}


/**
    This function displays an error message on stderr.  The error and the
    source of the error are given.

    \param  daErr   The error code to be processed.
    \param  source  A Pointer to a string representing the source of the error.
*/
VTVOID displayError(DAERR const daErr, VTCHAR const * const source)
{
    char buffer[ERROR_BUFFER_SIZE];
    /*
        While there is no way to know how big an error message is going to be,
        DAGetErrorString will truncate if needed.

        DAGetErrorString Parameters:
            1)  A DA related error code.
            2)  A pointer to a buffer to receive the error related message.
            3)  The size of the buffer in parameter 2.
    */
    DAGetErrorString(daErr, buffer, ERROR_BUFFER_SIZE);

    fprintf(stderr, "\n***  Error: %s in %s.  ***\n\n", buffer, source);
}


/*
    ---------------------------------------------------------------------------
    Primary CA processing.
    ---------------------------------------------------------------------------
*/


/**
    Exercise CA by processing the given document.  This function can be
    called recursively to process sub-documents (embeddings for example).

    \param  hDoc    A handle to the document to be processed.  This handle
                    is created by a call to DAOpenDocument or DAOpenTreeRecord.
	\param	outputFile	A pointer to stdout or the output file.

    \return A DA related error code.  DAERR_OK for success.
*/
DAERR useCA(VTHDOC const hDoc, FILE* outputFile)
{
    DAERR daErr;
    VTHCONTENT hcaContent;
    SCCCAGETCONTENT caContent;
    VTBYTE buffer[CONTENT_BUFFER_SIZE];
    VTBOOL bBigEndian = FALSE; /*this is only used if the -u flag is specified on the commandline.*/

    /*  Set the document specific options.  */
    daErr = setOptions(outputFile, hDoc);
    if (daErr != DAERR_OK)
    {
        /*  Notify that error occurred, but continue processing.  */
        displayError(daErr, "setOptions");
    }

    /*find the endian-ness of the system we're running on.*/
    {
        VTSHORT s = -2;
        VTBYTE * b = (VTBYTE *)&s;
        if ((b[0] == 0xFF) && (b[1] == 0xFE))
        {
            bBigEndian = TRUE;
        }
    }

    /*
        Initialize the CA content handle.  Successful calls to CAOpenContent
        should be matched by calls to CACloseContent.

        CAOpenContent Parameters:
            1)  A handle to the document to be processed.
            2)  Pointer to a handle that will receive a value uniquely
                identifying the document to the Content Access routines.

        CAOpenContent returns a DAERR code.  DAERR_OK indicates success.
    */
    daErr = CAOpenContent(hDoc, &hcaContent);
    if (daErr != DAERR_OK)
    {
        displayError(daErr, "CAOpenContent");
        return daErr;
    }

    /*
        Set up the structure used to return CA items.  Particularly, it is
        the consumer's responsibility to provide a buffer of at least 1K
        bytes.  SCCCA_GENERATED, and SCCCA_SHEET items require that the
        buffer be aligned on a 2 byte boundary.
    */
    caContent.dwStructSize = sizeof(SCCCAGETCONTENT);
    caContent.dwFlags      = 0;
    caContent.dwMaxBufSize = CONTENT_BUFFER_SIZE;
    caContent.pDataBuf     = buffer;

    /*
        Read the first CA item.  If successful, CAReadFirst will return the
        file properties and initial character set of the source.

        CAReadFirst Parameters:
            1)  The Content Access handle for the document as returned by
                CAOpenContent.
            2)  A pointer to a structure to be filled with CA data.
                CAReadFirst will fill this structure with SCCCA_FILEPROPERTY
                related information if successful.

        CAReadFirst returns a DAERR code.  DAERR_OK indicates success.
    */
    daErr = CAReadFirst(hcaContent, &caContent);
    if (daErr != DAERR_OK)
    {
        displayError(daErr, "CAReadFirst");
        CACloseContent(hcaContent);
        return daErr;
    }

    /*
        Process CA items.  During CA processing several error codes are of
        interest:

            DAERR_OK:   Successful creation of a CA item.
            DAERR_MISALIGNMENT:
                        pDataBuf was not aligned properly for the item CA
                        was trying to create.  CA will still provide
                        reasonable values in the content structure, and
                        processing can continue.
            DAERR_EOF:  End of file was reached.  This signals normal
                        successful completion.
    */
    while (daErr == DAERR_OK || daErr == DAERR_MISALIGNMENT)
    {
        switch (caContent.dwType)
        {
        case SCCCA_BEGINTAG:
            handleBeginTag(outputFile, &caContent);
            break;

        case SCCCA_ENDTAG:
            handleEndTag(outputFile, hDoc, &caContent);
            break;

        case SCCCA_BREAK:
            /*  Breaks are used internally and may be ignored.  */
            fprintf(outputFile, "SCCCA_BREAK.\n");
            break;

        case SCCCA_FILEPROPERTY:
            /*  File Id and initial character set.  */
            /*  When dwType is SCCCA_FILEPROPERTY:
                    dwSubType will be forced to be SCCCA_FILEID.
                    dwData1   will be the file type identifier.
                    dwData2   will be the initial character set of the input
                              document.
            */
            fprintf(outputFile, "SCCCA_FILEPROPERTY:\tFile Id = 0x%08x, Initial Character Set = 0x%08x.\n",
                caContent.dwData1, caContent.dwData2);
            break;

        case SCCCA_GENERATED:
            /*
                Generated text.  Generated text comes from the process of
                interpreting a document instead of from the source document
                itself.  Numbers in spreadsheets, or a formatted date are
                examples.

                SCCCA_GENERATED items will be returned with a
                DAERR_MISALIGNMENT error code if the buffer was not 2 byte
                aligned.  The values in caContent will still be reasonable
                to continue processing, but the generated text that might
                have been returned will not be.

                When dwType is SCCCA_GENERATED:
                    dwSubType     contains flags providing attribute
                                  information about the text.
                    dwData1       is the number of characters in the text.
                    dwData2       shows the input character set of the text.
                    dwDataBufSize contains the size of the text in bytes.
                    pDataBuf      points to the buffer containing the text.
            */
            if (daErr == DAERR_MISALIGNMENT)
            {
				displayError(daErr, "SCCCA_GENERATED");
            }
            if (caContent.dwSubType & SCCCA_XMPMETADATA)
            {
                fprintf(outputFile, "SCCCA_GENERATED:\tdwSubType = 0x%08x, Number of Characters = %i, Character Set = 0x%08x.\n",
                caContent.dwSubType, caContent.dwData1, caContent.dwData2);
            }
            else if (caContent.dwSubType & SCCCA_URLTEXT)
            {
                fprintf(outputFile, "SCCCA_GENERATED:\tdwSubType = 0x%08x, Number of Characters = %i, Character Set = 0x%08x.\n",
                caContent.dwSubType, caContent.dwData1, caContent.dwData2);
            }
            else
            {
                fprintf(outputFile, "SCCCA_GENERATED:\tdwSubType = 0x%08x, Number of Characters = %i, Character Set = 0x%08x.\n",
                caContent.dwSubType, caContent.dwData1, caContent.dwData2);
            }
            printCABuffer(outputFile, (VTBYTE*)(caContent.pDataBuf), caContent.dwDataBufSize, bBigEndian);
            break;

        case SCCCA_OBJECT:
            handleObject(outputFile, hDoc, &caContent);
            break;

        case SCCCA_SHEET:
            /*
                Sheet name (the name of a worksheet in a spreadsheet, or
                a slide in a presentation, for example).

                SCCCA_SHEET items will be returned with a DAERR_MISALIGNMENT
                error code if the buffer was not 2 byte aligned.  The values
                in caContent will still be reasonable to continue processing,
                but the sheet name that might have been returned will not be.

                When dwType is SCCCA_SHEET:
                    dwSubType     can contain flags further specifying the type
                                  of sheet.  
                    dwData1       is the number of characters in the text.
                    dwData2       shows the input character set of the text.
                    dwDataBufSize contains the size of the text in bytes.
                    pDataBuf      points to the buffer containing the text.
            */
            if (daErr == DAERR_MISALIGNMENT)
            {
				displayError(daErr, "SCCCA_SHEET");
            }
           
            fprintf(outputFile, "SCCCA_SHEET:\tdwSubType = 0x%08x, Number of Characters = %i, Character Set = 0x%08x.\n",
                caContent.dwSubType, caContent.dwData1, caContent.dwData2);
            printCABuffer(outputFile, (VTBYTE*)(caContent.pDataBuf), caContent.dwDataBufSize, bBigEndian);
            break;

        case SCCCA_STYLECHANGE:
            handleStyle(outputFile, &caContent, bBigEndian);
            break;

        case SCCCA_TEXT:
            /*
                Text.

                When dwType is SCCCA_TEXT:
                    dwSubType     contains flags providing attribute
                                  information about the text (SCCCA_BOLD
                                  to indicate that the text is bold, for
                                  example).
                    dwData1       is the number of characters in the text.
                    dwData2       shows the input character set of the text.
                    dwDataBufSize contains the size of the text in bytes.
                    pDataBuf      points to the buffer containing the text.
            */
            fprintf(outputFile, "SCCCA_TEXT:\tdwSubType = 0x%08x, Number of Characters = %i, Character Set = 0x%08x.\n",
                caContent.dwSubType, caContent.dwData1, caContent.dwData2);
            printCABuffer(outputFile, (VTBYTE*)(caContent.pDataBuf), caContent.dwDataBufSize, bBigEndian);
            break;

        case SCCCA_TREENODELOCATOR:
            /*
                Locator in archive document tree.  The SCCCA_TREENODELOCATOR
                provides information that can be used to access nodes in a
                archive document in random order.

                When dwType is SCCCA_TREENODELOCATOR:
                    dwData1     provides special flags used when opening a
                                random node from an archive.
                    dwData2     provides additional data used when opening a
                                random node from an archive.
                    dwData2     provides additional data used when opening a
                                random node from an archive.
            */
            fprintf(outputFile, "SCCCA_TREENODELOCATOR:\tSpecial Flags = 0x%08x, Param1 = 0x%08x, Param2 = 0x%08x.\n",
                caContent.dwData1, caContent.dwData2, caContent.dwData3);
            break;

        case SCCCA_ANNOTATION:
            fprintf(outputFile, "SCCCA_ANNOTATION:\tNote Type = 0x%08x, Note Tag Id = 0x%08x.\n",
                caContent.dwSubType, caContent.dwData1);
            break;

        case SCCCA_COMMENTREFERENCE:
            /*
                Reference to a comment.  The SCCCA_COMMENTREFERENCE
                will be output on the correct slide for presentations, in the 
                correct cell for spread sheets,or in the correct position in
                the text list for word processing documents.  

                When dwType is SCCCA_COMMENTREFERENCE:
                    dwData1     provides the type of comment. Possible values
                                are SCCCA_COMMENT_PARAGRAPH (0), SCCCA_COMMENT_CELL (1),
                                SCCCA_COMMENT_SLIDE (2), or SCCCA_COMMENT_VECTORPAGE (3).
                    dwData2     provides the ID of the SCCCA_SUBDOCTEXT begin tag/end 
                                tag pair surrounding the text of the comment.
            */
            fprintf(outputFile, "SCCCA_COMMENTREFERENCE:\tComment Type = 0x%08x, Comment Tag Id = 0x%08x.\n",
                caContent.dwData1, caContent.dwData2);
            break;

        case SCCCA_CELL:
        {
            VTBOOL hidden = FALSE;
            if (caContent.dwSubType & SCCCA_HIDDEN)
            {
                hidden = TRUE;
            }

            /*
                Delineates the beginning of a content for a specified cell or
                range of merged cells.  The given cell extends until another
                SCCCA_CELL, or SCCCA_SHEET.  Row and column information is 0 based.

                When dwType is SCCCA_CELL:
                    dwData1     provides the row of the starting cell for the range.
                    dwData2     provides the column of the starting cell for the range.
                    dwData3     provides the row of the ending cell for the range.
                    dwData4     provides the column of the ending cell for the range.

                If the starting row equals the ending row, and the starting
                column equals the ending column then a single cell is indicated
                instead of a merged range.
            */
            fprintf(outputFile, "SCCCA_CELL:\tStarting row = %i, Starting Column = %i, Ending row = %i, Ending column = %i, Hidden = %u.\n",
                caContent.dwData1, caContent.dwData2, caContent.dwData3, caContent.dwData4, hidden);
            break;
        }

        case SCCCA_RECORD:
            /*
                Delineates the beginning of a content for a record of archive file and email archive file

                When dwType is SCCCA_RECORD:
                    dwData1     reserved.
                    dwData2     reserved.
                    dwData3     reserved
                    dwData4     reserved

            */
            fprintf(outputFile, "SCCCA_RECORD:\n");
            break;

        case SCCCA_SLIDE:
            /* Delineates the beginning of content for 
               a presentation slide. The end of a slide
               is implied by the occurrence of another SCCCA_SLIDE item in
               this document (not in a subdocument such as an embedding), 
               or by the end of the items in this document. */
			if(caContent.dwData1 & SCCCA_SLIDEHIDDEN)
				fprintf(outputFile, "SCCCA_SLIDE \t (hidden).\n");
			else
				fprintf(outputFile, "SCCCA_SLIDE.\n");
            break;

        case SCCCA_OBJECTNAME:
            /* Name of the preceeding SCCCA_EMBEDDEDOBJECT.
               When dwType is SCCCA_OBJECTNAME:
                    dwData1       is the number of characters in the name.
                    dwData2       shows the input character set of the name.
                    dwDataBufSize contains the size of the name in bytes.
                    pDataBuf      points to the buffer containing the name.
            */
            fprintf(outputFile, "SCCCA_OBJECTNAME:\tdwSubType = 0x%08x, Number of Characters = %i, Character Set = 0x%08x.\n",
                caContent.dwSubType, caContent.dwData1, caContent.dwData2);
            printCABuffer(outputFile, (VTBYTE*)(caContent.pDataBuf), caContent.dwDataBufSize, bBigEndian);
            break;

        case SCCCA_OBJECTALTSTRING:
            /* Alt String of the preceeding SCCCA_EMBEDDEDOBJECT.
               When dwType is SCCCA_OBJECTALTSTRING:
                    dwData1       is the number of characters in the string.
                    dwData2       shows the input character set of the string.
                    dwDataBufSize contains the size of the string in bytes.
                    pDataBuf      points to the buffer containing the string.
            */
            fprintf(outputFile, "SCCCA_OBJECTALTSTRING:\tdwSubType = 0x%08x, Number of Characters = %i, Character Set = 0x%08x.\n",
                caContent.dwSubType, caContent.dwData1, caContent.dwData2);
            printCABuffer(outputFile, (VTBYTE*)(caContent.pDataBuf), caContent.dwDataBufSize, bBigEndian);
            break;

		case SCCCA_REVISION_USER:
            /* ID and name of users doing revisions
                    dwData1       User ID
                    pDataBuf      points to the buffer containing the user name.
            */			
            fprintf(outputFile, "SCCCA_REVISION_USER:\tUserID = 0x%08x.\n", caContent.dwData1);
            printCABuffer(outputFile, (VTBYTE*)(caContent.pDataBuf), caContent.dwDataBufSize, bBigEndian);
			break;

		case SCCCA_REVISION_SHEETNAME:
            /* ID and name of sheets referenced in revisions section
                    dwData1       Sheet ID
                    pDataBuf      points to the buffer containing the sheet name.
            */			
            fprintf(outputFile, "SCCCA_REVISION_SHEETNAME:\tSheetID = 0x%08x.\n", caContent.dwData1);
            printCABuffer(outputFile, (VTBYTE*)(caContent.pDataBuf), caContent.dwDataBufSize, bBigEndian);
			break;

		case SCCCA_REVISION_ROW:
			fprintf(outputFile, "SCCCA_REVISION_ROW:\tSheetID = 0x%08x, Start Row = 0x%08x, End Row = 0x%08x.\n", caContent.dwData1, caContent.dwData2, caContent.dwData3);
			break;

		case SCCCA_REVISION_CELL:
			fprintf(outputFile, "SCCCA_REVISION_CELL:\tSheetID = 0x%08x, Column = 0x%08x, Row = 0x%08x.\n", caContent.dwData1, caContent.dwData2, caContent.dwData3);
			break;

		case SCCCA_REVISION_COLUMN:
			fprintf(outputFile, "SCCCA_REVISION_COLUMN:\tSheetID = 0x%08x, Start Column = 0x%08x, End Column = 0x%08x.\n", caContent.dwData1, caContent.dwData2, caContent.dwData3);
			break;

		case SCCCA_REVISION_SHEET:
			fprintf(outputFile, "SCCCA_REVISION_SHEET:\tNew Sheet Name ID = 0x%08x, Old Sheet Name ID = 0x%08x.\n", caContent.dwData2, caContent.dwData3);
			break;
        default:
            /*  Unknown CA type.  */
            fprintf(outputFile, "\t***  Error:  Unexpected type (0x%08x) from CA.  ***\n", caContent.dwType);
            break;
        }

        /*
            Read the next CA item.

            CAReadNext Parameters:
                1)  The Content Access handle for the document as returned
                    by CAOpenContent.
                2)  A pointer to a structure to be filled with CA data.

            CAReadNext returns a DAERR code.
        */
        daErr = CAReadNext(hcaContent, &caContent);
    }

    fprintf(outputFile, "\n\n");

    /*  EOF is normal successful completion.  */
    if (daErr != DAERR_EOF)
    {
        displayError(daErr, "CAReadNext");
        CACloseContent(hcaContent);
        return daErr;
    }

    /*
        Close the CA content.

        CACloseContent Parameters:
            1)  The Content Access handle for the document as returned by
                CAOpenContent.

        CACloseContent returns a DAERR code.  DAERR_OK indicates success.
    */
    daErr = CACloseContent(hcaContent);
    if (daErr != DAERR_OK)
    {
        displayError(daErr, "CACloseContent");
    }

    return daErr;
}


/**
    This function provides handling of SCCCA_BEGINTAG CA items outputting
    all items of interest to various subtypes.

	\param	outputFile	A pointer to stdout or the output file.
    \param  caContent   A pointer to a structure containing CA output.  The
                        information in caContent must be related to a
                        SCCCA_BEGINTAG item.
*/
VTVOID handleBeginTag(FILE* outputFile, SCCCAGETCONTENT const * const caContent)
{
	VTCHAR tagName[CONTENT_NAME_SIZE + 1];	/* buffer for a tag name */

    fprintf(outputFile, "SCCCA_BEGINTAG:");
    /*  Begin (and end) tags have several interesting sub types.  */
    switch (caContent->dwSubType)
    {
    case SCCCA_DOCUMENTPROPERTY:
        /*  Document properties have ids.  */
        fprintf(outputFile, "\tSCCCA_DOCUMENTPROPERTY: Property Id = 0x%08x.", caContent->dwData1);
        break;

    case SCCCA_EMAILFIELD:
        /*  Email fields have ids.  */
        fprintf(outputFile, "\tSCCCA_EMAILFIELD:  Mail Field Id = 0x%08x.", caContent->dwData1);
        break;

    case SCCCA_EMAILTABLE:
        /*  Email tables have ids.  */
        fprintf(outputFile, "\tSCCCA_EMAILTABLE:  Mail Table Id = 0x%08x.", caContent->dwData1);
        break;

    case SCCCA_LINKEDOBJECT:
        /*
            Linked objects are attachments.  All information needed to open
            and process the attachment is provided both upon begin tag, and
            end tag.  We choose to process the attachment on end tag.  Any
            text associated with the attachment will appear between the
            begin and end tags.
        */
        fprintf(outputFile, "\tSCCCA_LINKEDOBJECT:  Object Id = 0x%08x, Flags = 0x%08x, Special Parameter #1 = 0x%08x, Special Parameter #2 = 0x%08x",
            caContent->dwData1, caContent->dwData2, caContent->dwData3, caContent->dwData4);
        break;

    case SCCCA_SUBDOCTEXT:
        /*
            Sub-document sections can have ids matching ids of SCCCA_ANNOTATION
            items.  The SCCCA_ANNOTATION will provide the type of the
            annotation while the SUBDOCTEXT tag will provide the content of the
            annotation.
        */
        fprintf(outputFile, "\tSCCCA_SUBDOCTEXT: Id = 0x%08x.", caContent->dwData1);
        break;

    case SCCCA_COMMENTREFERENCE:
        /*
            Comment references have a type (SCCCA_COMMENT_PARAGRAPH, SCCCA_COMMENT_CELL, SCCCA_COMMENT_SLIDE, or SCCCA_COMMENT_VECTORPAGE)
            and an id that matches the id of both SCCCA_SUBDOCTEXT and SCCCA_NOTE.  The SCCCA_SUBDOCTEXT
            can be defined anywhere in the file, but the SCCCA_COMMENTREFERENCE will always be put out
            in the correct paragraph, cell, or slide.
        */
        fprintf(outputFile, "\tSCCCA_COMMENTREFERENCE: Type = 0x%08x, Id = 0x%08x.", caContent->dwData1, caContent->dwData2);
        break;

    case SCCCA_SUBDOCPROPERTY:
        /*
            Sub document properties have a property id.
        */
        fprintf(outputFile, "\tSCCCA_SUBDOCPROPERTY: Property Id = 0x%08x.", caContent->dwData1);
        break;

	case SCCCA_STYLE:
		/*
            Styles may have flags.
        */
		fprintf(outputFile, "\tSCCCA_STYLE:");
		if(caContent->dwData1 & SCCCA_STYLEFLAG_INLINE_NUMBERING)
			fprintf(outputFile, "\tInline Number");
		break;

	case SCCCA_SS_REV_CREATE:
	case SCCCA_SS_REV_SAVE:
	case SCCCA_SS_REV_ADDROW:
	case SCCCA_SS_REV_DELETEROW:
	case SCCCA_SS_REV_INSERTCOL:
	case SCCCA_SS_REV_DELETECOL:
	case SCCCA_SS_REV_NEWCELL:
	case SCCCA_SS_REV_CLEARCELL:
	case SCCCA_SS_REV_MODIFYCELL:
	case SCCCA_SS_REV_MOVECELLS:
	case SCCCA_SS_REV_ADDSHEET:
	case SCCCA_SS_REV_RENAMESHEET:
	case SCCCA_SS_REV_FORMAT: /* will be followed by two SCCCA_REVISION_CELL upper left and lower right */

        /*
            Spread sheet revision information.
			The following items all have a Sequence ID, a User ID, and a DOS encoded Date/Time.
        */
		retrieveTagName(caContent->dwSubType, tagName);
        fprintf(outputFile, "\t%s: Sequence Id = 0x%08x, User Id = 0x%08x, Date and Time = 0x%08x.", tagName, caContent->dwData1, caContent->dwData2, caContent->dwData3);
        break;

	case SCCCA_SS_REVISIONS:
	case SCCCA_SS_USERNAMES:
	case SCCCA_SS_REV_OLDCELLCONTENT:
	case SCCCA_SS_REV_NEWCELLCONTENT:
	case SCCCA_SS_REV_OLDCELLLOCATION: /* will be followed by two SCCCA_REVISION_CELL upper left and lower right */
	case SCCCA_SS_REV_NEWCELLLOCATION: /* will be followed by two SCCCA_REVISION_CELL upper left and lower right */
	case SCCCA_SS_SHEETNAMES:
	case SCCCA_SS_SHEETNAME:
        /*
            Spread sheet revision information.
			The following items are generally containers of other data.
        */
		retrieveTagName(caContent->dwSubType, tagName);
        fprintf(outputFile, "\t%s.", tagName);
        break;

	case SCCCA_SS_USERNAME:
        /*
            Spread sheet revision User name.
			SCCCA_SS_USERNAME surrounds a user name and contains the user number in caContent->dwData1
        */
		retrieveTagName(caContent->dwSubType, tagName);
        fprintf(outputFile, "\t%s, User ID = 0x%08x.", tagName, caContent->dwData1);
        break;

    default:
        /*  All other begin tag subtypes have no extra information.  */
        fprintf(outputFile, "\tTag Type = 0x%08x.", caContent->dwSubType);
        break;
    }
    fprintf(outputFile, "\n");
}


/**
    This function provides handling of SCCCA_ENDTAG CA items by outputting
    all items of interest to various subtypes and recursively processing
    attachments.

	\param	outputFile	A pointer to stdout or the output file.
    \param  hDoc        A handle to the document being processed.
    \param  caContent   A pointer to a structure containing CA output.  The
                        information in caContent must be related to a
                        SCCCA_ENDTAG item.
*/
VTVOID handleEndTag(FILE* outputFile, VTHDOC const hDoc, SCCCAGETCONTENT const * const caContent)
{
	VTCHAR tagName[CONTENT_NAME_SIZE + 1];	/* buffer for a tag name */

    fprintf(outputFile, "SCCCA_ENDTAG:");
    /*  End (and begin) tags have several interesting sub types.  */
    switch (caContent->dwSubType)
    {
    case SCCCA_DOCUMENTPROPERTY:
        /*
            Document properties.  Ids were provided with begin tag but are
            not repeated on end tag.
        */
        fprintf(outputFile, "\tSCCCA_DOCUMENTPROPERTY:");
        break;

    case SCCCA_EMAILFIELD:
        /*
            Email field.  Ids were provided with begin tag but are not
            repeated on end tag.
        */
        fprintf(outputFile, "\tSCCCA_EMAILFIELD:");
        break;

    case SCCCA_EMAILTABLE:
        /*
            Email table.  Ids were provided with begin tag but are not
            repeated on end tag.
        */
        fprintf(outputFile, "\tSCCCA_EMAILTABLE:");
        break;

    case SCCCA_LINKEDOBJECT:
    /*
        Linked objects are attachments.  All information needed to open and
        process the attachment is provided both upon begin tag, and end tag.
        We choose to process the attachment on end tag.

        When dwType is SCCCA_ENDTAG and dwSubType is SCCCA_LINKEDOBJECT:
            dwData1 is the internal id of the sub document.
            dwData2 is an internal type for the sub document.
            dwData3 is a special parameter used for opening the sub document.
            dwData4 is a special parameter used for opening the sub document.
    */
    {
        /*
            DAOpenDocument expects a IOSPECLINKEDOBJECT when opening
            attachments.
        */
        IOSPECLINKEDOBJECT attachmentInformation;
        VTHDOC hSubDoc = VTHDOC_INVALID;
        DAERR daErr;

        fprintf(outputFile, "\tSCCCA_LINKEDOBJECT:  Object Id = 0x%08x, Flags = 0x%08x, Special Parameter #1 = 0x%08x, Special Parameter #2 = 0x%08x",
            caContent->dwData1, caContent->dwData2, caContent->dwData3, caContent->dwData4);

        /*
            Initialize the structure containing information about the
            attached document.
        */
        attachmentInformation.dwStructSize = sizeof(IOSPECLINKEDOBJECT);
        attachmentInformation.hDoc         = hDoc;
        attachmentInformation.dwObjectId   = caContent->dwData1;
        attachmentInformation.dwType       = caContent->dwData2;
        attachmentInformation.dwParam1     = caContent->dwData3;
        attachmentInformation.dwParam2     = caContent->dwData4;
        attachmentInformation.dwReserved1  = 0;
        attachmentInformation.dwReserved2  = 0;

        /*
            DAOpenDocument creates a handle to a document suitable for use in
            various Outside In API calls.  Successful calls to DAOpenDocument
            should be matched by calls to DACloseDocument.

            DAOpenDocument Parameters:
                1)  The address of a document handle that will contain the
                    created document handle when the call returns successfully.
                2)  Type indicating what information is being provided to
                    open the document (IOTYPE_LINKEDOBJECT in this case,
                    showing that we are opening an attachment).
                3)  A pointer to information needed to open the document (a
                    pointer to an initialized IOSPECLINKEDOBJECT in this case).
                4)  Flags.  The 0 for flags forces DA to identify the document
                    type for us.

            DAOpenDocument returns a DAERR code.  DAERR_OK indicates success.
        */
        daErr = DAOpenDocument(&hSubDoc, IOTYPE_LINKEDOBJECT, &attachmentInformation, 0);
        /*
            While processing sub documents we notify if errors occur, but we
            continue processing the remainder of the parent document.
        */
        if (daErr == DAERR_OK)
        {
			VTDWORD dwLength, dwFallbackId;
			/*  The fallback format isn't inherited by the linked document, so reset it */
			dwLength = sizeof(VTDWORD);
			if(DAGetOption(hDoc, SCCOPT_FALLBACKFORMAT, &dwFallbackId, &dwLength) == SCCERR_OK)
					DASetOption(hSubDoc, SCCOPT_FALLBACKFORMAT, &dwFallbackId, dwLength);

            /*
                Process the attachment.  This recursively calls useCA.  For
                normal document recursion into subdocuments is fairly safe
                as sub documents do not usually nest very deeply.  However,
                extreme cases of document nesting can occur, and recursion
                on sub documents may not be appropriate for all applications.
            */
            daErr = useCA(hSubDoc, outputFile);
            if (daErr != DAERR_OK)
            {
                displayError(daErr, "useCA");
            }
            DACloseDocument(hSubDoc);
        }
        else
        {
            displayError(daErr, "DAOpenDocument");
        }
        fprintf(outputFile, "\nEnd of SCCCA_LINKEDOBJECT.\n\n");
    }
    break;

    case SCCCA_SUBDOCTEXT:
        /*
            Sub-document section.  Ids were provided with begin tag but are
            not repeated on end tag.
        */
        fprintf(outputFile, "\tSCCCA_SUBDOCTEXT:");
        break;

    case SCCCA_COMMENTREFERENCE:
        /*
            Ids and type were provided with begin tag, but are not repeated on end tag.
        */
        fprintf(outputFile, "\tSCCCA_COMMENTREFERENCE:");
        break;

    case SCCCA_SUBDOCPROPERTY:
        /*
            Ids were provided with begin tag, but are not repeated on end tag.
        */
        fprintf(outputFile, "\tSCCCA_SUBDOCPROPERTY:");
        break;

    case SCCCA_STYLE:
        /*
            Styles have flags, but they are not repeated on end tag
        */
		fprintf(outputFile, "\tSCCCA_STYLE:");
		break;

	case SCCCA_SS_REV_CREATE:
	case SCCCA_SS_REV_SAVE:
	case SCCCA_SS_REV_ADDROW:
	case SCCCA_SS_REV_DELETEROW:
	case SCCCA_SS_REV_INSERTCOL:
	case SCCCA_SS_REV_DELETECOL:
	case SCCCA_SS_REV_NEWCELL:
	case SCCCA_SS_REV_CLEARCELL:
	case SCCCA_SS_REV_MODIFYCELL:
	case SCCCA_SS_REV_MOVECELLS:
	case SCCCA_SS_REV_ADDSHEET:
	case SCCCA_SS_REV_RENAMESHEET:
	case SCCCA_SS_REV_FORMAT: 
	case SCCCA_SS_REVISIONS:
	case SCCCA_SS_USERNAMES:
	case SCCCA_SS_USERNAME:
	case SCCCA_SS_REV_OLDCELLCONTENT:
	case SCCCA_SS_REV_NEWCELLCONTENT:
	case SCCCA_SS_REV_OLDCELLLOCATION:
	case SCCCA_SS_REV_NEWCELLLOCATION:
        /*
            Spread sheet revision information.
        */
		retrieveTagName(caContent->dwSubType, tagName);
        fprintf(outputFile, "\t%s.", tagName);
        break;

    default:
        /*  All other end tag subtypes have no extra information.  */
        fprintf(outputFile, "\tTag Type = 0x%08x.", caContent->dwSubType);
        break;
    }
    fprintf(outputFile, "\n");
}


/**
    This function provides handling of SCCCA_OBJECT CA items by outputting
    all interesting data, and recursively processing sub-documents.

	\param	outputFile	A pointer to stdout or the output file.
    \param  hDoc        A handle to the document being processed.
    \param  caContent   A pointer to a structure containing CA output.  The
                        information in caContent must be related to a
                        SCCCA_OBJECT item.
*/
VTVOID handleObject(FILE* outputFile, VTHDOC const hDoc, SCCCAGETCONTENT const * const caContent)
{
    fprintf(outputFile, "SCCCA_OBJECT:");
    /*  Objects have several interesting sub types.  */
    switch(caContent->dwSubType)
    {
    case SCCCA_EMBEDDEDOBJECT:
    /*  Embeddings.

        When dwType is SCCCA_OBJECT and dwSubType is SCCCA_EMBEDDEDOBJECT:
            dwData1 is the internal id of the sub document.
            dwData2 is the stream id of the alternative graphic object.
                    Typically the alternative graphic would be an icon or a
                    thumbnail image of the document.
            dwData3 is the stream id of an OLE object or CA_INVALIDITEM if
                    none is provided.
    */
    {
        /*
            DAOpenDocument expects a IOSPECSUBOBJECT when opening
            embeddings.
        */
        IOSPECSUBOBJECT embeddingInformation;
        VTHDOC hSubDoc = VTHDOC_INVALID;
        DAERR daErr = DAERR_OK;
        VTDWORD fileId = 0;

        fprintf(outputFile, "\tSCCCA_EMBEDDEDOBJECT: Object Id = 0x%08x, Alternate Graphic Id = 0x%08x, OLE Object Id = 0x%08x.\n",
            caContent->dwData1, caContent->dwData2, caContent->dwData3);
            
        /*
            Initialize the structure containing information about the embedded
            object.  dwStreamId will be conditionally set later.
        */
        embeddingInformation.dwStructSize = sizeof(IOSPECSUBOBJECT);
        embeddingInformation.hDoc         = hDoc;
        embeddingInformation.dwObjectId   = caContent->dwData1;
        embeddingInformation.dwReserved1  = 0;
        embeddingInformation.dwReserved2  = 0;

        /*
            The OLE object will provide more content if it is available
            so try it first.
        */
        if (caContent->dwData3 != CA_INVALIDITEM)
        {
            embeddingInformation.dwStreamId = caContent->dwData3;

            /*
                DAOpenDocument creates a handle to a document suitable for
                use in various Outside In API calls.  Successful calls to
                DAOpenDocument should be matched by calls to DACloseDocument.

                DAOpenDocument Parameters:
                    1)  The address of a document handle that will contain
                        the created document handle when the call returns
                        successfully.
                    2)  Type indicating what information is being provided to
                        open the document (IOTYPE_SUBOBJECT in this case,
                        showing that we are opening an embedding).
                    3)  A pointer to information needed to open the document
                        (a pointer to an initialized IOSPECSUBOBJECT in this
                        case).
                    4)  Flags.  The 0 for flags forces DA to identify the
                        document type for us.

                DAOpenDocument returns a DAERR code.  DAERR_OK indicates
                success.
            */
            daErr = DAOpenDocument(&hSubDoc, IOTYPE_SUBOBJECT, &embeddingInformation, 0);

            /*
                Attempt to identify the file type of the embedding.  If we
                can't identify then try the alternate graphic if available.
            */
            if (daErr == DAERR_OK)
            {
                /*
                    DAGetFileId attempts to identify the type of a document.

                    DAGetFileId Parameters:
                        1)  A handle to the document to be identified.
                        2)  A pointer to a VTDWORD which will be filled with
                            the file identifier on successful completion.

                    DAGetFileId returns a DAERR code.  DAERR_OK indicates
                    success.
                */
                daErr = DAGetFileId(hSubDoc, &fileId);
                if ((daErr != DAERR_OK) || (fileId == FI_NONE) || (fileId == FI_UNKNOWN) || 
					     (fileId == FI_7BITTEXT) || (fileId == FI_TEXT))
                {
                    /*
                        We failed in identifying the sub document using the
                        OLE id.  Indicate that we should try the alternative
                        graphic id instead by setting hSubDoc to
                        VTHDOC_INVALID.
                    */
                    DACloseDocument(hSubDoc);
                    hSubDoc = VTHDOC_INVALID;
                }
            }
        }

        /*  Now try the graphic if needed.  */
        if ((hSubDoc == VTHDOC_INVALID) && (caContent->dwData2 != CA_INVALIDITEM))
        {
            fileId = 0;
            embeddingInformation.dwStreamId = caContent->dwData2;
            daErr = DAOpenDocument(&hSubDoc, IOTYPE_SUBOBJECT, &embeddingInformation, 0);
        }

        /*
            While processing sub documents we notify if errors occur, but we
            continue processing the remainder of the parent document.
        */
        if (daErr == DAERR_OK)
        {
            if(hSubDoc != VTHDOC_INVALID)
            {
                /* check the file id, if the file id is progressive JPEG, skip it */
                if (fileId == 0)
                    daErr = DAGetFileId(hSubDoc, &fileId);
                if (fileId != FI_PROGRESSIVEJPEG)
                {
                    /*
                        Process the embedding.  This recursively calls useCA.
                        For normal document recursion into subdocuments is fairly
                        safe as sub documents do not usually nest very deeply.
                        However, extreme cases of document nesting can occur, and
                        recursion on sub documents may not be appropriate for all
                        applications.
                    */
                    daErr = useCA(hSubDoc, outputFile);
                    if (daErr != DAERR_OK)
                    {
                        displayError(daErr, "useCA");
                    }
                }
                DACloseDocument(hSubDoc);
            }
        }
        else
        {
            displayError(daErr, "DAOpenDocument");
        }
        fprintf(outputFile, "\nEnd of SCCCA_OBJECT::SCCCA_EMBEDDEDOBJECT.\n");
    }
    break;

    case SCCCA_ARCHIVEITEMCONTAINER:
        /*  Archive container.  */
        fprintf(outputFile, "\tArchive Subobject (0x%08x): Node Id = 0x%08x, Stream Id = 0x%08x, Flags = 0x%08x\n", 
            caContent->dwSubType, caContent->dwData1, caContent->dwData2, caContent->dwData4);
        break;

    case SCCCA_COMPRESSEDFILE:
    case SCCCA_MESSAGE:
    case SCCCA_CONTACT:
    case SCCCA_CALENDARENTRY:
    case SCCCA_NOTE:
    case SCCCA_TASK:
    case SCCCA_JOURNALENTRY:
    case SCCCA_ATTACHMENT:
    case SCCCA_FIELDSFILE:
    /*  Archived objects.

        When dwType is SCCCA_ENDTAG and dwSubType is one of the archived items:
            dwData1 is the id of the node of the sub document.
            dwData2 is the stream id for the sub document.
    */
    {
        VTHDOC hSubDoc = VTHDOC_INVALID;
        DAERR daErr;

        fprintf(outputFile, "\tArchive Subobject (0x%08x): Node Id = 0x%08x, Stream Id = 0x%08x, Flags = 0x%08x\n", 
            caContent->dwSubType, caContent->dwData1, caContent->dwData2, caContent->dwData4);

        /*
            DAOpenTreeRecord opens a specified sub-document from an archive.
            The resulting handle is suitable for use in various Outside In API
            calls.  Successful calls to DAOpenTreeRecord should be matched by
            calls to DACloseTreeRecord.

            DAOpenTreeRecord Parameters:
                1)  The handle of the parent archive document containing the
                    sub-document to be opened,
                2)  The address of a document handle that will contain the
                    created document handle when the call returns successfully,
                3)  The index of the sub-document to be opened.  This index
                    will be provided by the CA API.

            DAOpenTreeRecord returns a DAERR code.  DAERR_OK indicates success.
        */
        daErr = DAOpenTreeRecord(hDoc, &hSubDoc, caContent->dwData1);
       /*
            While processing sub documents we notify if errors occur, but we
            continue processing the remainder of the parent document.
        */
        if (daErr == DAERR_OK)
        {
            /*
                Process the attachment.  This recursively calls useCA.  For
                normal document recursion into subdocuments is fairly safe as
                sub documents do not usually nest very deeply.  However,
                extreme cases of document nesting can occur, and recursion on
                sub documents may not be appropriate for all applications.
            */
            daErr = useCA(hSubDoc, outputFile);
            if (daErr != DAERR_OK)
            {
                displayError(daErr, "useCA");
            }
            DACloseTreeRecord(hSubDoc);
        }
        else
        {
            displayError(daErr, "DAOpenTreeRecord");
        }
        fprintf(outputFile, "\nEnd of SCCCA_OBJECT::Archive Subobject.\n");
    }
    break;

    default:
        /*  Unknown sub type for object.  */
        fprintf(outputFile, "\t***  Error:  Unexpected subtype (0x%08x) for SCCCA_OBJECT.  ***\n",
            caContent->dwSubType);
        break;
    }
}


/**
    This function provides handling of SCCCA_STYLECHANGE CA items by
    outputting all interesting data.

	\param	outputFile	A pointer to stdout or the output file.
    \param  caContent   A pointer to a structure containing CA output.  The
                        information in caContent must be related to a
                        SCCCA_STYLECHANGE item.
*/
VTVOID handleStyle(FILE* outputFile, SCCCAGETCONTENT const * const caContent, VTBOOL const bBigEndian)
{
    fprintf(outputFile, "SCCCA_STYLECHANGE:");
    /*  Style changes have several interesting sub types.  */
    switch(caContent->dwSubType)
    {
    case SCCCA_PARASTYLE:
        /*
            The name of the style is provided.

            When dwType is SCCCA_STYLECHANGE and dwSubType is SCCCA_PARASTYLE:
                dwData1 is the id of the style.
                dwDataBufSize contains the size of the text in bytes.
        */
        fprintf(outputFile, "\tSCCCA_PARASTYLE: Style Id = 0x%08x.\n", caContent->dwData1);
        /*  Not all styles are named.  */
        if (caContent->dwDataBufSize > 0)
        {
            printCABuffer(outputFile, (VTBYTE*)(caContent->pDataBuf), caContent->dwDataBufSize, bBigEndian);
        }
        else
        {
            fprintf(outputFile, "\t<No Name>.\n");
        }
        break;

    case SCCCA_CHARSTYLE:
        /*
            While character style is a valid sub type, no interesting data
            is provided.
        */
        fprintf(outputFile, "\tSCCCA_CHARSTYLE.\n");
        break;

    case SCCCA_HEIGHTANDSPACING:
        /*
            The height and/or spacing of the style are provided.  Values are
            given in twips.

            When dwType is SCCCA_STYLECHANGE and dwSubType is
            SCCCA_HEIGHTANDSPACING:
                dwData1 is any combination of SCCCA_HEIGHT and SCCCA_SPACING
                        OR-ed together.
                dwData2 contains the character height if the SCCCA_HEIGHT
                        flag is set.
                dwData3 contains the line spacing if the SCCCA_SPACING flag
                        is set.
        */
        fprintf(outputFile, "\tSCCCA_HEIGHTANDSPACING:");
        if (caContent->dwData1 & SCCCA_HEIGHT)
        {
            fprintf(outputFile, " Height = %i", caContent->dwData2);
        }
        if (caContent->dwData1 & SCCCA_SPACING)
        {
            fprintf(outputFile, " Spacing = %i", caContent->dwData3);
        }
        fprintf(outputFile, ".\n");
        break;

    case SCCCA_INDENTS:
        /*
            The left and/or right and/or first line indents of the style are
            provided.  Values are given in twips.

            When dwType is SCCCA_STYLECHANGE and dwSubType is SCCCA_INDENTS:
                dwData1 is any combination of SCCCA_LEFTINDENT,
                        SCCCA_RIGHTINDENT, and SCCCA_FIRSTINDENT OR-ed
                        together.
                dwData2 contains the left indent if the SCCCA_LEFTINDENT
                        flag is set.
                dwData3 contains the right indent if the SCCCA_RIGHTINDENT
                        flag is set.
                dwData4 contains the first line indent if the
                        SCCCA_FIRSTINDENT flag is set.
        */
        fprintf(outputFile, "\tSCCCA_INDENTS:");
        if (caContent->dwData1 & SCCCA_LEFTINDENT)
        {
            fprintf(outputFile, " Left Indent = %i", caContent->dwData2);
        }
        if (caContent->dwData1 & SCCCA_RIGHTINDENT)
        {
            fprintf(outputFile, " Right Indent = %i", caContent->dwData3);
        }
        if (caContent->dwData1 & SCCCA_FIRSTINDENT)
        {
            fprintf(outputFile, " First Line Indent = %i", caContent->dwData4);
        }
        fprintf(outputFile, ".\n");
        break;

    case SCCCA_OCE:
        /*
            The source document's original character encoding is provided.

            When dwType is SCCCA_STYLECHANGE and dwSubType is SCCCA_OCE:
                dwData1 is the source document's character encoding.
        */
        /* If a style is created that doesn't contain a charset(no CHANGE_CHARSET within 
        the style), then the OCE will be the default value, which is 0x00000000.  Similarly, if 
        the style doesn't contain a charset, we shouldn't put one out.*/
        if(caContent->dwData1 != 0x00000000)
        {
            fprintf(outputFile, "\tSCCCA_OCE: Original Encoding = 0x%08x.\n", caContent->dwData1);
        }
		else
		{    /* output a newline to avoid concatenations */
			 fprintf(outputFile, "\n");
		}
        break;

    default:
        /*  Unknown sub type for style change.  */
        fprintf(outputFile, "\t***  Error:  Unexpected subtype (0x%08x) for SCCCA_STYLECHANGE.  ***\n",
            caContent->dwSubType);
        break;
    }
}
/**
    This function retrieves a string representing the name of a tag

    \param  dwTag		ID of the tag
	\param	name		string to retrieve the name of the tag
*/
VTVOID retrieveTagName(VTDWORD const dwTag, VTCHAR * name)
{
	/* rearranged this to make the Linux compiler stop throwing warnings
	and to make it a bit more efficient.  jrw  4/22/14 */

	const char *s = NULL;
	switch(dwTag)
	{
		case SCCCA_SS_REVISIONS:
			s = "SCCCA_SS_REVISIONS";
			break;
		case SCCCA_SS_USERNAMES:
			s = "SCCCA_SS_USERNAMES";
			break;
		case SCCCA_SS_REV_CREATE:
			s = "SCCCA_SS_REV_CREATE";
			break;
		case SCCCA_SS_REV_SAVE:
			s = "SCCCA_SS_REV_SAVE";
			break;
		case SCCCA_SS_REV_ADDROW:
			s = "SCCCA_SS_REV_ADDROW";
			break;
		case SCCCA_SS_REV_DELETEROW:
			s = "SCCCA_SS_REV_DELETEROW";
			break;
		case SCCCA_SS_REV_INSERTCOL:
			s = "SCCCA_SS_REV_INSERTCOL";
			break;
		case SCCCA_SS_REV_DELETECOL:
			s = "SCCCA_SS_REV_DELETECOL";
			break;
		case SCCCA_SS_REV_NEWCELL:
			s = "SCCCA_SS_REV_NEWCELL";
			break;
		case SCCCA_SS_REV_CLEARCELL:
			s = "SCCCA_SS_REV_CLEARCELL";
			break;
		case SCCCA_SS_REV_MODIFYCELL:
			s = "SCCCA_SS_REV_MODIFYCELL";
			break;
		case SCCCA_SS_REV_OLDCELLCONTENT:
			s = "SCCCA_SS_REV_OLDCELLCONTENT";
			break;
		case SCCCA_SS_REV_NEWCELLCONTENT:
			s = "SCCCA_SS_REV_NEWCELLCONTENT";
			break;
		case SCCCA_SS_REV_MOVECELLS:
			s = "SCCCA_SS_REV_MOVECELLS";
			break;
		case SCCCA_SS_REV_OLDCELLLOCATION:
			s = "SCCCA_SS_REV_OLDCELLLOCATION";
			break;
		case SCCCA_SS_REV_NEWCELLLOCATION:
			s = "SCCCA_SS_REV_NEWCELLLOCATION";
			break;
		case SCCCA_SS_REV_ADDSHEET:
			s = "SCCCA_SS_REV_ADDSHEET";
			break;
		case SCCCA_SS_REV_RENAMESHEET:
			s = "SCCCA_SS_REV_RENAMESHEET";
			break;
		case SCCCA_SS_REV_FORMAT:
			s = "SCCCA_SS_REV_FORMAT";
			break;
		case SCCCA_SS_USERNAME:
			s = "SCCCA_SS_USERNAME";
			break;
		case SCCCA_SS_SHEETNAMES:
			s = "SCCCA_SS_SHEETNAMES";
			break;
		case SCCCA_SS_SHEETNAME:
			s = "SCCCA_SS_SHEETNAME";
			break;
	}
	if (s)
		strncpy((char *)name, s, CONTENT_NAME_SIZE);

}

