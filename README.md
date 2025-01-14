# qrgenerator

## Description

This program creates a QR code with the string given from the
command line, then:<br />
 - It prints the QR code on the terminal.
 - It saves the QR code as a PNG file.
<br /><br />
```
// You can verify the generated code with:
sudo apt install zbar-tools
zbarimg qrcode.png
```


## Requirement
* Qrencode<br />
Used for producing QRcodes.
```
sudo apt install libqrencode-dev
// Or, on Termux (Android)
pkg install libqrencode
```

* PNG Library
Used for producing QRcodes.
```
sudo apt install libpng-dev
// Or, on Termux
pkg install libpng
```

## Commands
```
// Run
make
./qrencode <string to convert>
```


## Screenshots
### Linux
<img src="screenshots/example-linux.png" width="500" />

### Termux (Android)
<p>
	<img src="screenshots/example-termux1.png" width="400"  />
	<img src="screenshots/example-termux2.png" width="400" />
</p>
<img src="screenshots/example-termux3.png" width="400" />
