 This is a proof of concept repository for attempting to introduce a new way to deat with media via interpolation tables. Originally this work was tested on Linux systems but "should" work on Windows. However, there is no gurantee that this project works but should be a good starting point for implementing interpolation type approaches particularly from GSL.

 - `Modelica` is a modelica test library

 - `data` directory contains the externally generated lookup tables for various media

 - `noname` is the actual implementation of an interpolation approach based on the functionality in GSL (P.S. this name needs to change...)
 
 - `doc` contains various documentation. External Function Interface starts in section 12.9 (pp. 165-176) of the Modelica Specification in the `doc` directory. The specification can also be found at https://www.modelica.org/documents.
 
 - `Sources` is the directory `ExternalMedia/Projects/Sources`. It contains the original .cpp and .h files as reference.