# glt

GLT is a that translates OpenGL calls to equivalent OpenGL ES 3.2 calls.

Implementation status (Roughly estimated):

| OpenGL                        | Core Profile | Core Profile (extensions) | Compatibility Profile | Compatibility Profile (extensions) |
|-------------------------------|--------------|---------------------------|-----------------------|------------------------------------|
| Available on GLES core        | 345          | -                         | -                     | -                                  |
| Available on GLES extensions  | 30           | -                         | -                     | -                                  |
| Emulated with GLES core       | 57           | -                         | -                     | -                                  |
| Emulated with GLES extensions | 2            | -                         | -                     | -                                  |
| Emulated with C               | 9            | -                         | -                     | -                                  |
| Unimplemented                 | 251          | -                         | -                     | -                                  |
