# glt

GLT is a that translates OpenGL calls to equivalent OpenGL ES 3.2 calls.

Implementation status:

| OpenGL                        | Core Profile | Core Profile (extensions) | Compatibility Profile | Compatibility Profile (extensions) |
|-------------------------------|--------------|---------------------------|-----------------------|------------------------------------|
| Available on GLES core        | 343          | -                         | -                     | -                                  |
| Available on GLES extensions  | 30           | -                         | -                     | -                                  |
| Emulated with GLES core       | 14           | -                         | -                     | -                                  |
| Emulated with GLES extensions | 5            | -                         | -                     | -                                  |
| Emulated with C               | 0            | -                         | -                     | -                                  |
| Unimplemented                 | 303          | -                         | -                     | -                                  |

Some specific functionality that are not covered by GLES may still be classified as "Available on GLES".
