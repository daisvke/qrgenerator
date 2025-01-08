#include "qrgenerator.hpp"

int main(int argc, char *argv[]) {
	// Parse arguments
	if (argc < 2 || !argv[1][0]) {
		std::cerr << "Missing argument." << std::endl;
		exit(1);
	}

	std::string	inputString = argv[1];
	
    try
    {
		// Generate a QR code from the input string
        QRcode *qrcode = QRcode_encodeString(
				inputString.c_str(), 0, QR_ECLEVEL_L, QR_MODE_8, 1
			);

        if (qrcode) {
			// The output file has to be a PNG file
            std::string filetype = ".png";
            std::string filename = OTP_QRCODE_FILE + filetype;

			printQRCode(qrcode); // Print the QR code on the terminal
            saveQRCodeAsPNG(qrcode, filename.c_str()); // Save the QR code as PNG
            QRcode_free(qrcode);
        } else
            throw QRCodeGenerationException();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

	return 0;
}
