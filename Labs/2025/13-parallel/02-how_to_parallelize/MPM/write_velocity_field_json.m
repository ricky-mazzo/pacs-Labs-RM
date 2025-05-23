
hx = hy = 0.02;
nx = ny = 2/hx;
in.grid_properties = struct ('hx', hx, 'hy', hy, 'nx', nx, 'ny', ny);
num_global_nodes = (in.grid_properties.nx+1)*(in.grid_properties.ny+1);

x = 0 : hx : nx*hx;
y = 0 : hy : ny*hy;

[x, y] = meshgrid (x, y);

vx = 1e-3*(2*pi*(1-y))(:);
vy = 1e-3*(2*pi*(x-1))(:);

in.num_particles = 100_000;
in.x = randn(in.num_particles, 1) * .2/6 + 1.5;
in.y = randn(in.num_particles, 1) * .2/6 + 1.5;
in.iprops.label = [0:numel(in.x)](:);

in.dprops = struct ('M',  2*pi*(.4/6)^2*ones(in.num_particles, 1)/in.num_particles, 'VX', zeros(in.num_particles, 1), 'VY', zeros(in.num_particles, 1));
in.grid_vars = struct ('vx', vx, 'vy', vy, 'rho', zeros(num_global_nodes, 1));

str = jsonencode (in);
fid = fopen ('velocity.json', 'w');
fwrite (fid, str, 'char');
fclose (fid);
