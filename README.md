# image-compression-ADSP-21489

## Project Overview
The main goal of this project is to implement a grayscale image compression system on the ADSP-21489 development platform using the Discrete Cosine Transform (DCT). The compression is based on the JPEG standard, but only selected steps of the method are implemented.

## Project-specific implementation steps:

1.	Convert an RGB image to grayscale and load it onto the development board.
2.	Center pixel values around zero, segment the image, and apply DCT quantization.
       - The quantization matrix can be chosen arbitrarily; note that it affects the compression ratio and quality.
    - The same matrix is used for decoding.

4.	Apply the zig-zag scan and encode the DC and AC coefficients.
5.	Entropy coding is not required.
6.	Serialize the obtained results in a chosen output format (e.g. .dct).
7.	Analyze the results in Python or MATLAB by computing the MSE compared to the original image.

## Workflow:
1.	Load the RGB image onto the ADSP-21489 and convert it to grayscale.
2.	Center pixel values and divide the image into blocks.
3.	Apply the DCT and perform quantization.
4.	Perform the zig-zag scan and encode the AC and DC coefficients.
5.	Serialize the results in the chosen format.
6.	Analyze the results in Python (or MATLAB).
7.	Identify bottlenecks and measure the number of cycles.
8.	Optimize memory usage and execution speed

### Additional:
*	Display processing progress using the on-board LEDs.

## Hardware and software requirements
### Hardware
*	Development platform: ADSP-21489
### Software
*	CrossCore Embedded Studio (CCES) 2.12.1 – C development and loading the .dxe file to the board
*	Python – result analysis (decompression and MSE computation)
*	Git – cloning the repository

## References
*	JPEG compression standard
*	DCT theory
*	DSP development on ADSP-21489

# How to use
## 1) Prerequisites
Before running the project, make sure you have:
*	an **ADSP-21489** development board with the required cables,
*	**CrossCore Embedded Studio 2.12.1** installed,
*	**Python** installed (for result analysis),
*	**Git** installed (for cloning the repository

## 2) Image preparation
*	Choose an image you want to compress (**.bmp, 200×200, 24-bit**).
*	Copy the image into the **Debug** folder of the Image_compression project.
*	In the **main** file, set the filename of the image to be read (e.g., "img1.bmp").
*	Run the program and wait for the successful completion message. If any errors occur, check the console output and verify the input file and parameters.

## 3)  Running the project in CrossCore Embedded Studio
*	The Git repository contains the complete Image_compression project—clone it locally.
*	Open Image_compression in CCES and then run:
-	    Project → Clean
-	    Project → Build
*	After a successful build, the Debug folder should contain Image_compression.dxe.
*	Run the application on the board (via the Run/Debug configuration).
*	After successful execution, the following outputs are generated:
    - a grayscale image in .bmp format,
    -  a generated .dct file required for analysis


## 4) Result analysis (Python)
*	Create a local folder for analysis.
*	Copy the following into that folder:
    - the Python script (decode_dct.py),
    - the original image (.bmp),
    - the generated output file (.dct).
*	Open CMD in that folder and run:
    - py decode_dct.py image_name.dct image_name.bmp
*	The results (reconstructed image, MSE value, and visualizations) will appear immediately and can be saved if needed.


