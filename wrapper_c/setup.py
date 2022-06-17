from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize
from Cython.Distutils import build_ext

ext_module = [
    Extension(
        "wrapper",
        sources=["wrapper.pyx", "interface.c"],  # our Cython source
        language="c",  # generate C code
        include_dirs=["../inc/crlf", "../inc/crlf/jni1.6", "../inc/crlf/jni1.6/win32"],
        library_dirs=["../lib/w64/VisualStudio2019/c"],
        libraries=[
            "dex40c",
            "dmd40c",
            "dsa40c",
            "ekd40c",
            "emp40c",
            "esd40c",
            "etb40c",
            "etne40c",
            "lib40c",
            "tra40c",
            "msvcmrt",
        ],
        extra_compile_args=["/EHsc"],
    )
]

setup(
    name="wrapper",
    cmdclass={"build_ext": build_ext},
    ext_modules=cythonize(ext_module),
)
