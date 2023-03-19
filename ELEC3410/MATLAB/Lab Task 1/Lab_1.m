amp = 2.5;

wn = 15;

B = 3/wn;

s = tf('s');

G = 36.5/(s^2+10.7*s);

Gi = G^-1;

Fq = (B*s+1)/(s/wn+1)^3;

Q = Fq*Gi;

C = Q/(1-Q*G);

[num,den] = tfdata(C,'v');

y = num(1)/den(1);

C_inf = 1/C - 1/y;

[num1,den1] = tfdata(C_inf,'v');



