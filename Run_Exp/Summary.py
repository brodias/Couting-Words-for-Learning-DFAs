import numpy as np

L_SYMB_28_n_8 = [min(1000,1000+528.769),min(1000,1000+557.997),min(1000,1000+560.163),min(1000,1000+552.396),min(1000,1000+554.546),min(1000,1000+532.066),min(1000,1000+531.105),min(1000,1000+523.763),min(1000,1000+540.928),min(1000,1000+538.679)]
start = 0
not_start = 10
timeout = 10
l = len(L_SYMB_28_n_8)
mean = round(np.mean(L_SYMB_28_n_8),3)
var = round(np.std(L_SYMB_28_n_8),3)
print(str(8),':','mean:',mean,', var:',var,', nb_exp:',l,', timeout:',timeout,', start_ok:',start,', start_not_ok:',not_start)
