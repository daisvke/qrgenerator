#include "qrgenerator.hpp"

// Print the QR code on the terminal
void printQRCode(QRcode *qrcode, int scale = 1) {
    // Define Dimensions for the image
    int size = qrcode->width;
    int width = size * scale; // Factor to scale up each QR code module for better readability.
    int margin = 1 * scale; // Space around the QR code, scaled for better readability.
    int total_size = width + 2 * margin; // Total width size.

    /*
     * Fill and write each row
     *
     * Outer loop (y): Iterates over rows.
     * Inner loop (x): Iterates over pixels in the row.
     */

	for (int y = 0; y < total_size; ++y) {
		for (int x = 0; x < total_size; ++x) {
			if (x < margin || x >= total_size - margin ||
				y < margin || y >= total_size - margin)
			{
				std::cout << "█"; // White border
			} else {
            	// Determine if the pixel is within the QR code area.
				int module_x = (x - margin) / scale;
				int module_y = (y - margin) / scale;

				bool isModule = (module_x >= 0 && module_x < size &&
								 module_y >= 0 && module_y < size &&
								 (qrcode->data[module_y * size + module_x] & 0x01));

            	// Print the pixel as an ASCII square.
				isModule ? std::cout << " " : std::cout << "█";
			}
		}
		std::cout << std::endl;
	}
}

/*
 * The generated output file should have a size of:
 *  (QR_width×scale+2×margin)×(QR_width×scale+2×margin).
 * For a scale of 10 and margin of 4 modules,
 * it will be sufficiently large and compatible with most QR code scanners.
 */
void saveQRCodeAsPNG(QRcode *qrcode, const char *filename, int scale = OTP_QRCODE_SCALE) {
    // Define Dimensions for the image
    int size = qrcode->width;
    int png_width = size * scale; // Factor to scale up each QR code module for better readability.
    int margin = 4 * scale; // Space around the QR code, scaled for better readability.
    int total_size = png_width + 2 * margin; // Total width size of the PNG image (square)

    // Open the outfile in binary write mode.
    FILE *fp = fopen(filename, "wb");
    if (!fp)
        throw OpenFileException();

    /*
     * Initialize libpng
     *
     * png_create_write_struct: Initializes the PNG write structure.
     * PNG_LIBPNG_VER_STRING: Uses the version of libpng linked at compile time.
     */
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        fclose(fp);
        throw LibpngInitException();
    }

    // Initialize the PNG info structure.
    png_infop info = png_create_info_struct(png);
    if (!info) {
        // Clean up if info structure creation fails.
        png_destroy_write_struct(&png, nullptr);
        fclose(fp);
        throw LibpngInitException();
    }

    /* 
     * setjmp: Sets a point for error recovery.
     *  If an error occurs during PNG creation, execution jumps here.
     *
     * png_jmpbuf: Retrieves the jump buffer for the PNG structure.
     */
    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        throw PNGCreationException();
    }

    png_init_io(png, fp);

    // Write PNG header
    png_set_IHDR(
        png, info, total_size, total_size, 8,
        PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT
        );
    // Write the header to the PNG file
    png_write_info(png, info);

    // Allocate row memory (png_bytep: a pointer type for PNG data)
    png_bytep row = (png_bytep)malloc(total_size);
    if (!row) {
        png_destroy_write_struct(&png, &info);
        throw PNGRowMemAllocationException();
    }

    /*
     * Fill and write each row
     *
     * Outer loop (y): Iterates over rows.
     * Inner loop (x): Iterates over pixels in the row.
     */

	for (int y = 0; y < total_size; ++y) {
		for (int x = 0; x < total_size; ++x) {
			if (x < margin || x >= total_size - margin ||
				y < margin || y >= total_size - margin)
			{
				row[x] = 0xFF; // White border
			} else {
            	// Determine if the pixel is within the QR code area.
				int module_x = (x - margin) / scale;
				int module_y = (y - margin) / scale;

				bool isModule = (module_x >= 0 && module_x < size &&
								 module_y >= 0 && module_y < size &&
								 (qrcode->data[module_y * size + module_x] & 0x01));

            	// Set pixel color (black: 0x00, white: 0xFF).
				row[x] = isModule ? 0x00 : 0xFF;
			}
		}
        // Write the row to the PNG file.
		png_write_row(png, row);
	}

    free(row);

    // Finish and clean up
    png_write_end(png, nullptr); // Finalize the PNG file
    png_destroy_write_struct(&png, &info); // Clean up libpng structures
    fclose(fp); // Close the file
}

int main() {
bool verbose=true; std::string secret = "abcd";
	if (verbose)
		std::cout << " Generating QR code..." << std::endl;
    try
    { 
        QRcode *qrcode = QRcode_encodeString(secret.c_str(), 0, QR_ECLEVEL_L, QR_MODE_8, 1);

        if (qrcode) {
            std::string filetype = ".png";
            std::string filename = OTP_QRCODE_FILE + filetype;

			printQRCode(qrcode); // Print the QR code on the terminal
            saveQRCodeAsPNG(qrcode, filename.c_str()); // Save the QR code as PNG
			if (verbose)
				std::cout << " Saved QR code as PNG file: '"
					<< filename << "'." << std::endl;
            QRcode_free(qrcode);
        } else {
            throw OpenFileException();
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}