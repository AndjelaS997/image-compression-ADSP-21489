# image-compression-ADSP-21489

## Project Overview
The main goal of this project is to implement a grayscale image compression system on the ADSP-21489 development platform using the Discrete Cosine Transform (DCT). The compression is based on the JPEG standard, but only selected steps of the method are implemented.

## Project-specific implementation steps:

1.	Convert an RGB image to grayscale and load it onto the development board.
2.	Center pixel values around zero, segment the image, and apply DCT quantization.
     o	The quantization matrix can be chosen arbitrarily; note that it affects the compression ratio and quality.
  	  o	The same matrix is used for decoding
4.	Apply the zig-zag scan and encode the DC and AC coefficients.
5.	Entropy coding is not required.
6.	Serialize the obtained results in a chosen output format (e.g., .dct).
7.	Analyze the results in Python or MATLAB by computing the MSE compared to the original image.
