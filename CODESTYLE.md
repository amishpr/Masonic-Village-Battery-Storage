# Code Style

This project uses GNU C Coding Standard.

More information about the GNU C Coding Standard can be found here:

https://www.gnu.org/prep/standards/html_node/Writing-C.html

# Formatting

To format our code, we will be using the program `indent`.

More information about `indent` can be found here:

https://www.gnu.org/software/indent/manual/indent.html


## Using indent

`indent` is a terminal application.


```bash
indent [options] [input-files]

indent [options] [single-input-file] [-o output-file]
```

### Example

#### macOS

```bash
indent example.c
```

#### Linux
```bash
indent -kr example.c
```
