close all; clear all; clc;

baseDir = "dataset/acquisizione01/";
fName = fullfile( baseDir, "008_008.RAW" );

fr = fopen( fName, "rb" );
data = fread( fr, 'int16' );
fclose( fr );

Fs = 16000; % 16 kHz

t_all = (0:length( data )-1)*(1/Fs);

fig = figure();
    plot( t_all, data );


[f, Y]  = myfft( data, Fs );
fig = figure();
    plot( f, Y );