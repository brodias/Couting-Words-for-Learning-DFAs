import numpy as np

L_SYMB_08_n_7 = [min(1000,1000+161.323),min(1000,18.247+163.296),min(1000,15.911+152.938),min(1000,25.893+164.164),min(1000,1000+187.927),min(1000,31.081+324.677),min(1000,72.573+385.401),min(1000,1000+317.613),min(1000,23.098+206.749),min(1000,102.531+192.213)]
l = len(L_SYMB_08_n_7)
mean = round(np.mean(L_SYMB_08_n_7),3)
var = round(np.std(L_SYMB_08_n_7),3)
print(str(7),':','mean:',mean,', var:',var,', nb_exp:',l)
