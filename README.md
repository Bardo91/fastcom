![](https://raw.githubusercontent.com/Bardo91/fastcom/master/doc/fastcom_logo.png)

[![Releases](https://img.shields.io/github/release/bardo91/fastcom.svg)](https://github.com/bardo91/fastcom/releases)  [![Issues](https://img.shields.io/github/issues/bardo91/fastcom.svg)](https://github.com/bardo91/fastcom/issues)  [![Stars](https://img.shields.io/github/stars/bardo91/fastcom.svg)](https://github.com/bardo91/fastcom/stars)

[![Build Status](https://travis-ci.org/Bardo91/fastcom.svg?branch=master)](https://travis-ci.org/Bardo91/fastcom)



# Installation

Clone the repository into any folder, and create a build folder for the compilation

```
git clone https://github.com/Bardo91/fastcom
cd fastcom && mkdir build && cd build
```

Configure with cmake and compile the project

```
cmake .. && make
```

Install fastcom to use it at any project you want.
```
sudo make install
```

Create fastcom mole IP environment variable. If you are using fastcom in the same computer, this step is not necessary. If you are using multiple computers, this variable should have the IP of where the first fastcom app is instantiated.
```
echo "export FASTCOM_MOLE_IP=\"127.0.0.1\"" >> ~/.bashrc
```

