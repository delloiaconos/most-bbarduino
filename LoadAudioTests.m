close all; clear all; clc;

baseDir = "dataset/acquisizione01/";
fName = fullfile( baseDir, "008_008.RAW" );

fr = fopen( fName, "rb" );
signal = fread( fr, 'int16' );
fclose( fr );

Fs = 16000; % 16 kHz

t_all = (0:length( signal )-1)*(1/Fs);


len = length( signal );
sx = signal( 1:2:len );

fig = figure();
    plot( t_all, signal );


[f, Y]  = myfft( sx, Fs );
fig = figure();
    plot( f, Y );

player = audioplayer( sx, Fs );
play( player )
