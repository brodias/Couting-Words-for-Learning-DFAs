import numpy as np

L_SYMB_15_n_8 = [min(1000,1000+402.973),min(1000,1000+402.667),min(1000,1000+421.162),min(1000,1000+442.682),min(1000,1000+445.054),min(1000,1000+460.86),min(1000,1000+426.406),min(1000,1000+389.189),min(1000,1000+455.171),min(1000,1000+471.87)]
l = len(L_SYMB_15_n_8)
mean = round(np.mean(L_SYMB_15_n_8),3)
var = round(np.std(L_SYMB_15_n_8),3)
print(str(8),':','mean:',mean,', var:',var,', nb_exp:',l)
