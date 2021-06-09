# Removing-periodic-noise-from-an-image
removing noise by Fourier transforming the image and filtering it in frequency space.

## Variables
  ### SIZE
  The size of the input image must be N x N (N = `SIZE`).  
  
  ### FILTER_SIZE
  Filtering sets the gray level of the noise and its surrounding pixels to 0. `FILTER_SIZE` is its size.  
  
  ### noise[]
  `noise[]` represents the coordinates of the noise that you want to remove in the power spectrum image.  
  Note: The top left corner of the image is the origin, the x-axis is positive for horizontal direction right, and the y-axis is positive for vertical direction down.
