################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/\ quant.c \
../src/Image_compression.c \
../src/blocks8.c \
../src/bmp_io.c \
../src/dct8.c \
../src/rgb_gray.c \
../src/zigzag_rle.c 

SRC_OBJS += \
./src/\ quant.doj \
./src/Image_compression.doj \
./src/blocks8.doj \
./src/bmp_io.doj \
./src/dct8.doj \
./src/rgb_gray.doj \
./src/zigzag_rle.doj 

C_DEPS += \
./src/\ quant.d \
./src/Image_compression.d \
./src/blocks8.d \
./src/bmp_io.d \
./src/dct8.d \
./src/rgb_gray.d \
./src/zigzag_rle.d 


# Each subdirectory must supply rules for building sources it contributes
src/\ quant.doj: ../src/\ quant.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="Image_compression" -proc ADSP-21489 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -O -Ov100 -g -D_DEBUG -DDO_CYCLE_COUNTS -DCORE0 @includes-848672b36c324398160417a890c03ede.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -swc -gnu-style-dependencies -MD -Mo "src/ quant.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Image_compression.doj: ../src/Image_compression.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="Image_compression" -proc ADSP-21489 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -O -Ov100 -g -D_DEBUG -DDO_CYCLE_COUNTS -DCORE0 @includes-848672b36c324398160417a890c03ede.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -swc -gnu-style-dependencies -MD -Mo "src/Image_compression.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/blocks8.doj: ../src/blocks8.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="Image_compression" -proc ADSP-21489 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -O -Ov100 -g -D_DEBUG -DDO_CYCLE_COUNTS -DCORE0 @includes-848672b36c324398160417a890c03ede.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -swc -gnu-style-dependencies -MD -Mo "src/blocks8.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/bmp_io.doj: ../src/bmp_io.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="Image_compression" -proc ADSP-21489 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -O -Ov100 -g -D_DEBUG -DDO_CYCLE_COUNTS -DCORE0 @includes-848672b36c324398160417a890c03ede.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -swc -gnu-style-dependencies -MD -Mo "src/bmp_io.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/dct8.doj: ../src/dct8.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="Image_compression" -proc ADSP-21489 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -O -Ov100 -g -D_DEBUG -DDO_CYCLE_COUNTS -DCORE0 @includes-848672b36c324398160417a890c03ede.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -swc -gnu-style-dependencies -MD -Mo "src/dct8.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/rgb_gray.doj: ../src/rgb_gray.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="Image_compression" -proc ADSP-21489 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -O -Ov100 -g -D_DEBUG -DDO_CYCLE_COUNTS -DCORE0 @includes-848672b36c324398160417a890c03ede.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -swc -gnu-style-dependencies -MD -Mo "src/rgb_gray.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/zigzag_rle.doj: ../src/zigzag_rle.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="Image_compression" -proc ADSP-21489 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -O -Ov100 -g -D_DEBUG -DDO_CYCLE_COUNTS -DCORE0 @includes-848672b36c324398160417a890c03ede.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -swc -gnu-style-dependencies -MD -Mo "src/zigzag_rle.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


