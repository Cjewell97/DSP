#ifndef ECE486_FIR
#define ECE486_FIR

/* struct definition
h : array to hold coefficients of fir
old : array to hold n_coef previous values of input
blocksize : size of input block
marker : keep track of starting index for output queue
*/
typedef struct your_fir_struct {
	float *h;
	float *old;
	int n_coef;
	int blocksize;
	int marker;
} FIR_T;

FIR_T *init_fir(
  float *fir_coefs,     //!< [in] Pointer to filter coefficient array
  int   n_coef,         //!< [in] Total number of filter coefficients
  int   blocksize	//!< [in] Block size for input sample blocks
);

void calc_fir(
  FIR_T *s,      	//!< [in,out] Pointer to previously initialized FIR filter structure,
                        //!< as provided by init_fir() (and possibly modified by 
		        //!< previous calls to calc_fir() )
  float *x,             //!< [in] pointer to the input sample array
  float *y              //!< [out] Pointer to an array for storage of output samples
);

void destroy_fir(
  FIR_T *s		//!< [in] Pointer to previously initialized FIR filter 
			//!< structure, provided by init_fir()
);

#endif

