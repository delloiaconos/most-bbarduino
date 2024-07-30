# MOST BlackBox Arduino-Based

This repository contains python source code for the Arduino-based Black Box Prototype.
Here are also included the python and MATLAB scripts, needed for the processing of the data recorded with the prototype.

This project is an output of SPOKE 5 WP4 "KEY ENABLING TOPICS", WP4.1 "Sensors, IoT, ICT, V2X", T4.1.4 "Design of a black box (for standard bikes) with sensors and LPWAN modules".
Additional results and information about this projects can be found in the deliverables:

- D4.1.4 - BlackBox Architecture
- D4.1.4 - BlackBox Prototype

## Project Structure

### Directories

This is how the project direcotry should look like:
```console
+---most-bbarduino
|  +---Application
|  |   +---MicrophoneTests.ino
|  |   +---DataLogging_AccGyrLidar.ino
|  +---DataProcessing
|  |   +---XXX
|  |   :
|  |   \---ZZZ
|  +---dataset
|  |   +---XXX
|  |   :
|  |   \---ZZZ
|  +---outputs
|      +---XXX
|      :
|      \---ZZZ
```

- `dataset` and `output`: directories are not versioned in this repository because they contain data and processing outputs.
- `DataProcessing`: post-processing scripts for data acquired from Arduino-Based Black Box Prototype.
- `ArduinoApplication`: post-processing scripts for data acquired from Arduino-Based Black Box Prototype.
- `Application`: source code for the tests and final application for the Arduino Platform.


## Contributors

- Salvatore Dello Iacono, DII University of Brescia
- Chiara Franzoni, University of Brescia

## Fundings

This study was carried out within the MOST-Sustainable Mobility National Research Center and received funding from the European Union Next-GenerationEU (PIANO NAZIONALE DI RIPRESA E RESILIENZA (PNRR) - MISSIONE 4 COMPONENTE 2, 
INVESTIMENTO 1.4 - D.D. 1033 17/06/2022, CN00000023), Spoke 5 "Light Vehicle and Active Mobility".