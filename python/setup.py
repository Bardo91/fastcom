from distutils.core import setup
from distutils.extension import Extension
import sys,os

setup_dict = dict(
name='fastcom',
version='1.0',
description='Fast communication utils for python and c++',
author='Pablo Ramon Soria',
author_email='pabramsor@gmail.com',
url='',
download_url='',
packages=['fastcom'],
package_dir={
    'fastcom' : 'fastcom',
},

long_description='Fast communication utils for python and c++',

license="MIT",
classifiers=[
    'Development Status :: 5 - Production/Stable',
    'Intended Audience :: Developers',
    'Intended Audience :: Education',
    'Intended Audience :: End Users/Desktop',
    'Intended Audience :: Information Technology',
    'Intended Audience :: Science/Research',
    'License :: OSI Approved :: MIT License',
    'Operating System :: OS Independent',
    'Programming Language :: C',
    'Programming Language :: C++',
    'Programming Language :: Python',
    'Programming Language :: Python :: 3',
    'Topic :: Communications',
    'Topic :: Utilities',
],
)

try:
    setup(**setup_dict)
except KeyboardInterrupt:
    raise
except:
    # If there are any compilation errors or there are no build tools available
    # for the extension module, delete the extension module and try to install
    # the pure Python version.
    setup(**setup_dict)