# Emofishes (Emotional Fishes)
## Introduction

Emofishes is a collection of proof of concepts that help improve, bypass or detect virtualized execution environments (focusing on the ones setup for malware analysis). Part of the code in all of the tools is based on [Alberto Ortega](https://github.com/a0rtega/pafish) Pafish (Paranoid Fish) project.

### Cufish (Curious Fish)

The environments used to perform behavioural analysis of malware need to be stealth, being able to avoid detection by malware is a must because it will spare the researcher or incident responder precious time in the analysis of the incident. Cufish aims to help with that, providing information about the execution environment that could be used pre-emptively to improve the sandbox and avoid detection by malware. You can [download](https://github.com/serializingme/emofishes/raw/master/dist/cufish.exe) the compiled executable.

## Build it Yourself

All code is written in C and can be built with MinGW. To compile you will likely need to use:

```
(If compiling in Windows systems)
> make -f Makefile.win

(If cross-compiling in Linux systems for a 32 bits Windows)
$ make -f Makefile.linux-32

(If cross-compiling in Linux systems for a 64 bits Windows, not always available)
$ make -f Makefile.linux-64
```

## Licensing

All code is licensed under GNU/GPL version 3. Icons used have been created by [Fast Icon Design](http://fasticon.com/).
