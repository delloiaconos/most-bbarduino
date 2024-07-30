function [f,Y] = myfft(signal,fs)
    L = length(signal);

    if rem( L, 2 ) == 1
        signal = signal(1:end-1);
        L = length( signal);
    end
    Y = fft( signal );
    P2 = abs(Y/L);
    P1 = P2(1:L/2+1);
    P1(2:end-1) = 2*P1(2:end-1);
    f = fs*(0:(L/2))/L;
    
    Y = P1;

end

