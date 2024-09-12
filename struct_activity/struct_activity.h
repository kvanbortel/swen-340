// Header file for the structs and pointers activity
// L. Kiser June 4, 2020

typedef struct {
	char id[2] ;	// simple point identifer of two alpha numeric characters.
					// This id[2] would have 6 bytes of padding bytes after it! This allows member x
					// to start on an even multiple of 8 for efficient operation.
	unsigned char version ;	// single byte version number -- uses one byte of that padding!
	double x ; // x-coordinate
	double y ; // y-coordinate
} POINT_t; 

// Create a typedef of a struct with these elements:
// unsigned char sequence
// int product_id
// float price
//
// Give it this name PRODUCT_t



