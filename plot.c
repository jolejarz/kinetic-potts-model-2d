/**************************************************************
***************************************************************
*** 
***   plot.c
***
***   Jason Olejarz
***   2011-01-27
***
***   This file contains all graphics functions for ising3d.
***
***************************************************************
**************************************************************/

#include "includes.h"

void animate_real_time(short int *spins, int *flips, double *energy, double *time, int steps_begin, int steps, double time_between_frames, int nx, int ny, double delay, double time_offset, short int save_frames, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot)
{
	int i;
	int x, y, z;
	
	fprintf(matlab_data, "%d\n%d\n%d\n%d\n%d\n%lf\n", nx, ny, steps_begin, steps, (int)save_frames, (double)time_offset);
	
	for (i=0; i<nx*ny; i++)
	{
		fprintf(matlab_data, "%d\n", *(spins+i));
	}
	
	fprintf(matlab_data, "%d\n", (int)(*energy));
	
	for (i=0; i<steps; i++)
	{
		y=(int)((*(flips+2*i))/nx);
		x=(int)(*(flips+2*i)-y*nx);

		fprintf(matlab_data, "%d %d\n", x, y);
		fprintf(matlab_data, "%d\n", *(flips+2*i+1));
		fprintf(matlab_data, "%d\n", (int)(*(energy+i+1)));
		fprintf(matlab_data, "%lf\n", (double)(*(time+i+1)));
	}
	
	fprintf(matlab_plot, "hold on\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "plotdata=fopen('%s', 'r');\n", matlab_data_filename);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "nx=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "ny=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "steps_begin=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "steps=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "save_frames=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "time_offset=fscanf(plotdata, '%%f', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "s=zeros(nx,ny);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for y=1:ny\n");
	fprintf(matlab_plot, "	for x=1:nx\n");
	fprintf(matlab_plot, "		s(x,y)=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "energy=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "last_time=0;\n");
	fprintf(matlab_plot, "title(['energy/spin = ', num2str(energy/nx/ny), ',  time = ', num2str(time_offset)], 'FontSize', 20)\n");
	fprintf(matlab_plot, "drawnow\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "imagecount=0;\n");
	fprintf(matlab_plot, "if save_frames==1\n");
	fprintf(matlab_plot, "  print('-djpeg', strcat(num2str(imagecount, '%%03d'), '.jpg'));\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for i=1:steps\n");
	fprintf(matlab_plot, "	x=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	y=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	flip=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	energy=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	time=fscanf(plotdata, '%%f', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	s(x+1,y+1)=flip;\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	if s(x+1,y+1)==0\n");
	fprintf(matlab_plot, "		h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], 'r', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "	elseif s(x+1,y+1)==1\n");
	fprintf(matlab_plot, "		h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], 'g', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "	elseif s(x+1,y+1)==2\n");
	fprintf(matlab_plot, "		h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], 'b', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "	elseif s(x+1,y+1)==3\n");
	fprintf(matlab_plot, "		h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], 'y', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "	elseif s(x+1,y+1)==4\n");
	fprintf(matlab_plot, "		h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], 'm', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "	elseif s(x+1,y+1)==5\n");
	fprintf(matlab_plot, "		h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], 'c', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "	elseif s(x+1,y+1)==6\n");
	fprintf(matlab_plot, "		h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], [1 0.55 0], 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "	elseif s(x+1,y+1)==7\n");
	fprintf(matlab_plot, "		h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], [0.55 0.27 0.07], 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "	else\n");
	fprintf(matlab_plot, "		h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], [0.5 0.5 0.5], 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	delete(h(x+1, y+1));\n");
	fprintf(matlab_plot, "	h(x+1, y+1)=h_temp;\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	if time-last_time>=%lf || i==steps\n",time_between_frames);
	fprintf(matlab_plot, "		title(['energy/spin = ', num2str(energy/nx/ny), ',  time = ', num2str(time_offset+time)], 'FontSize', 20)\n");
	fprintf(matlab_plot, "		pause(%lf)\n", delay);
	fprintf(matlab_plot, "		drawnow\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		last_time=last_time+%lf;\n",time_between_frames);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "          imagecount=imagecount+1;\n");
	fprintf(matlab_plot, "          if save_frames==1\n");
	fprintf(matlab_plot, "                  print('-djpeg', strcat(num2str(imagecount, '%%03d'), '.jpg'));\n");
	fprintf(matlab_plot, "          end\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "fclose(plotdata);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "hold off\n");

	return;
}

void animate_real_time_2(short int *spins, int *flips, double *energy, double *time, int steps, double starting_time, double time_between_frames, int nx, int ny, double delay, short int save_frames, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot)
{
	int i;
	int x, y;
	
	fprintf(matlab_data, "%d\n%d\n%d\n%d\n", nx, ny, steps, (int)save_frames);
	
	for (i=0; i<nx*ny; i++)
	{
		fprintf(matlab_data, "%d\n", *(spins+i));
	}
	
	fprintf(matlab_data, "%d\n", (int)(*energy));
	fprintf(matlab_data, "%lf\n", (double)(*time));
	
	for (i=0; i<steps; i++)
	{
		y=(int)((*(flips+2*i))/nx);
		x=(int)(*(flips+2*i)-y*nx);

		fprintf(matlab_data, "%d %d\n", x, y);
		fprintf(matlab_data, "%d\n", *(flips+2*i+1));
		fprintf(matlab_data, "%d\n", (int)(*(energy+i+1)));
		fprintf(matlab_data, "%lf\n", (double)(*(time+i+1)));
	}
	
	fprintf(matlab_plot, "hold on\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "plotdata=fopen('%s', 'r');\n", matlab_data_filename);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "nx=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "ny=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "steps=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "save_frames=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "s=zeros(nx,ny);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for y=1:ny\n");
	fprintf(matlab_plot, "	for x=1:nx\n");
	fprintf(matlab_plot, "		s(x,y)=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "energy=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "time=fscanf(plotdata, '%%f', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "last_time=time;\n");
	fprintf(matlab_plot, "title(['energy/spin = ', num2str(energy/nx/ny), ',  time = ', num2str(time)], 'FontSize', 20)\n");
	fprintf(matlab_plot, "drawnow\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "imagecount=0;\n");
	fprintf(matlab_plot, "if save_frames==1\n");
	fprintf(matlab_plot, "  print('-djpeg', strcat(num2str(imagecount, '%%03d'), '.jpg'));\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for i=1:steps\n");
	fprintf(matlab_plot, "	x=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	y=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	flip=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	energy=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	time=fscanf(plotdata, '%%f', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	s(x+1,y+1)=flip;\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	if s(x+1,y+1)==0\n");
	fprintf(matlab_plot, "		h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], 'r', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "	elseif s(x+1,y+1)==1\n");
	fprintf(matlab_plot, "		h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], 'g', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "	elseif s(x+1,y+1)==2\n");
	fprintf(matlab_plot, "		h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], 'b', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "	elseif s(x+1,y+1)==3\n");
	fprintf(matlab_plot, "		h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], 'y', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "	elseif s(x+1,y+1)==4\n");
	fprintf(matlab_plot, "		h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], 'm', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "	elseif s(x+1,y+1)==5\n");
	fprintf(matlab_plot, "		h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], 'c', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "	elseif s(x+1,y+1)==6\n");
	fprintf(matlab_plot, "		h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], [1 0.55 0], 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "	elseif s(x+1,y+1)==7\n");
	fprintf(matlab_plot, "		h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], [0.55 0.27 0.07], 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "	else\n");
	fprintf(matlab_plot, "		h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], [0.5 0.5 0.5], 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	delete(h(x+1, y+1));\n");
	fprintf(matlab_plot, "	h(x+1, y+1)=h_temp;\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	if time-last_time>=%lf || i==steps\n",time_between_frames);
	fprintf(matlab_plot, "		title(['energy/spin = ', num2str(energy/nx/ny), ',  time = ', num2str(time)], 'FontSize', 20)\n");
	fprintf(matlab_plot, "		pause(%lf)\n", delay);
	fprintf(matlab_plot, "		drawnow\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		last_time=last_time+%lf;\n",time_between_frames);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "          imagecount=imagecount+1;\n");
	fprintf(matlab_plot, "          if save_frames==1\n");
	fprintf(matlab_plot, "                  print('-djpeg', strcat(num2str(imagecount, '%%03d'), '.jpg'));\n");
	fprintf(matlab_plot, "          end\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "fclose(plotdata);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "hold off\n");

	return;
}

void animate_step(short int *spins, int *flips, int *flips_backward, int steps, int nx, int ny, int xmin, int xmax, int ymin, int ymax, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot)
{
	int i;
	int x, y;
	
	fprintf(matlab_data, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n", nx, ny, steps, xmin, xmax, ymin, ymax);
	
	for (i=0; i<nx*ny; i++)
	{
		fprintf(matlab_data, "%d\n", *(spins+i));
	}
	
	for (i=0; i<steps; i++)
	{
		y=(int)((*(flips+2*i))/nx);
		x=(int)(*(flips+2*i)-y*nx);

		fprintf(matlab_data, "%d %d\n", x, y);
		fprintf(matlab_data, "%d\n", *(flips+2*i+1));
		fprintf(matlab_data, "%d\n", *(flips_backward+2*i+1));
	}

	fprintf(matlab_plot, "function p_an\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "global nx ny steps xmin xmax ymin ymax old_step s h x_array y_array flip_array flip_backward_array energy_array time_array\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "plotdata=fopen('%s', 'r');\n", matlab_data_filename);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "nx=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "ny=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "steps=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "xmin=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "xmax=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "ymin=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "ymax=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "h=zeros(nx,ny);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "s=zeros(nx,ny);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "figure\n");
	fprintf(matlab_plot, "axis([0 nx 0 ny], 'equal')\n");
	fprintf(matlab_plot, "set(gca,'Units','centimeters')\n");
	fprintf(matlab_plot, "position=get(gcf,'Position');\n");
	fprintf(matlab_plot, "position(4)=500;\n");
	fprintf(matlab_plot, "set(gcf,'Position',position)\n");
	fprintf(matlab_plot, "xlabel('x')\n");
	fprintf(matlab_plot, "ylabel('y')\n");
	fprintf(matlab_plot, "title(['lattice=(', num2str(nx), ', ', num2str(ny), '), steps (min, current, max)=(', num2str(0), ', ', num2str(0), ', ', num2str(steps), ')'])\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "slider=uicontrol('Style','slider','Callback',@slider_callback,'Min',0,'Max',steps,'SliderStep',[1/steps 1],'Value',0,'Position',[64 400 384 20]);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for y=0:ny-1\n");
	fprintf(matlab_plot, "	for x=0:nx-1\n");
	fprintf(matlab_plot, "		s(x+1, y+1)=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "		if s(x+1, y+1)==0\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'r', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		elseif s(x+1, y+1)==1\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'g', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		elseif s(x+1, y+1)==2\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'b', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		elseif s(x+1, y+1)==3\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'y', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		elseif s(x+1, y+1)==4\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'm', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		elseif s(x+1, y+1)==5\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'c', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		elseif s(x+1, y+1)==6\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [1 0.55 0], 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		elseif s(x+1, y+1)==7\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [0.55 0.27 0.07], 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [0.5 0.5 0.5], 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "x_array=zeros(steps,1);\n");
	fprintf(matlab_plot, "y_array=zeros(steps,1);\n");
	fprintf(matlab_plot, "flip_array=zeros(steps,1);\n");
	fprintf(matlab_plot, "flip_backward_array=zeros(steps,1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "old_step=0;\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for i=1:steps\n");
	fprintf(matlab_plot, "	x_array(i)=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	y_array(i)=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	flip_array(i)=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	flip_backward_array(i)=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "function slider_callback(h_slider, eventdata)\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "global nx ny steps xmin xmax ymin ymax old_step s h x_array y_array flip_array flip_backward_array energy_array time_array\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "set(h_slider,'Value',cast(get(h_slider,'Value'),'int32'))\n");
	fprintf(matlab_plot, "title(['lattice=(', num2str(nx), ', ', num2str(ny), '), steps (min, current, max)=(', num2str(0), ', ', num2str(get(h_slider,'Value')), ', ', num2str(steps), ')'])\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "if get(h_slider,'Value')~=old_step\n");
	fprintf(matlab_plot, "	if get(h_slider,'Value')<old_step\n");
	fprintf(matlab_plot, "		loop_start=old_step;\n");
	fprintf(matlab_plot, "		loop_end=get(h_slider,'Value')+1;\n");
	fprintf(matlab_plot, "		loop_direction=-1;\n");
	fprintf(matlab_plot, "	elseif get(h_slider,'Value')>old_step\n");
	fprintf(matlab_plot, "		loop_start=old_step+1;\n");
	fprintf(matlab_plot, "		loop_end=get(h_slider,'Value');\n");	
	fprintf(matlab_plot, "		loop_direction=1;\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	flip_within_region=0;\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	i=loop_start;\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	flag=true;\n");
	fprintf(matlab_plot, "	while flag\n");
	fprintf(matlab_plot, "		x=x_array(i);\n");
	fprintf(matlab_plot, "		y=y_array(i);\n");
	fprintf(matlab_plot, "		if loop_direction==1\n");
	fprintf(matlab_plot, "			flip=flip_array(i);\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			flip=flip_backward_array(i);\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		s(x+1,y+1)=flip;\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		if s(x+1,y+1)==0\n");
	fprintf(matlab_plot, "			h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], 'r', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		elseif s(x+1,y+1)==1\n");
	fprintf(matlab_plot, "			h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], 'g', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		elseif s(x+1,y+1)==2\n");
	fprintf(matlab_plot, "			h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], 'b', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		elseif s(x+1,y+1)==3\n");
	fprintf(matlab_plot, "			h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], 'y', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		elseif s(x+1,y+1)==4\n");
	fprintf(matlab_plot, "			h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], 'm', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		elseif s(x+1,y+1)==5\n");
	fprintf(matlab_plot, "			h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], 'c', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		elseif s(x+1,y+1)==6\n");
	fprintf(matlab_plot, "			h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], [1 0.55 0], 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		elseif s(x+1,y+1)==7\n");
	fprintf(matlab_plot, "			h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], [0.55 0.27 0.07], 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			h_temp=patch([x x+1 x+1 x], [y y y+1 y+1], [0.5 0.5 0.5], 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		delete(h(x+1, y+1));\n");
	fprintf(matlab_plot, "		h(x+1, y+1)=h_temp;\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		if (x>=xmin && x<=xmax && y>=ymin && y<=ymax) || (i==1 && loop_direction==-1) || (i==steps && loop_direction==1)\n");
	fprintf(matlab_plot, "			flip_within_region=1;\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "		if loop_start==loop_end && flip_within_region==0\n");
	fprintf(matlab_plot, "			loop_start=loop_start+loop_direction;\n");
	fprintf(matlab_plot, "			loop_end=loop_end+loop_direction;\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		if i==loop_end\n");
	fprintf(matlab_plot, "			flag=false;\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			i=i+loop_direction;\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	if loop_direction==1\n");
	fprintf(matlab_plot, "		set(h_slider,'Value',cast(i,'int32'))\n");
	fprintf(matlab_plot, "	else\n");
	fprintf(matlab_plot, "		set(h_slider,'Value',cast(i-1,'int32'))\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "	title(['lattice=(', num2str(nx), ', ', num2str(ny), '), steps (min, current, max)=(', num2str(0), ', ', num2str(get(h_slider,'Value')), ', ', num2str(steps), ')'])\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "old_step=get(h_slider,'Value');\n");

	return;
}

void blinker_region_plot_matlab(struct run_parameters *p, char matlab_data_filename_1[], FILE *matlab_data_1, FILE *matlab_plot_1, char matlab_data_filename_2[], FILE *matlab_data_2, FILE *matlab_plot_2, char matlab_data_filename_3[], FILE *matlab_data_3, FILE *matlab_plot_3)
{
	int x, y;
	int nxtimesny=(p->xlen)*(p->ylen);
	
	static unsigned long int rng_seed_backup;
	static unsigned long int rng_long_int_backup;
	static double rng_double_backup;

	short int *spins_backup;
	short int *state_backup;
	int *map_backup[11+MAX_Q*(MAX_Q-1)];
	int *index_backup[11+MAX_Q*(MAX_Q-1)];
	double prob_backup[11+MAX_Q*(MAX_Q-1)];
	int coordinate_backup;
	short int move_backup;
	unsigned long int steps_backup;
	double time_backup;
	double energy_backup;
	int mag_backup;
	int num_value_backup[MAX_Q];
	short int *T_vertices_backup;
	short int *T_vertices_2_backup;

	short int *spins_plus_field;
	short int *spins_minus_field;

	short int *blinker_regions;

	// This array has length xlen*ylen.
	// It is used to determine the two possible values that the spins in each blinker region can take.
	short int *blinker_region_values[2];

	short int bias;

	short int eligible_spins;

	short int run_force;

	int i, j, k;

	short int q=p->q;

	// Allocate memory.
	spins_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
	state_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
	for (i=0; i<11+q*(q-1); i++)
	{
		map_backup[i]=(int*)malloc (sizeof(int)*(p->xlen)*(p->ylen));
		index_backup[i]=(int*)malloc (sizeof(int)*(1+(p->xlen)*(p->ylen)));
	}
	T_vertices_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
	T_vertices_2_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));

	// These are used for storing the lattice after application of plus and minus bias fields.
	spins_plus_field=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
	spins_minus_field=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));

	// This is used for saving the blinker regions.
	// A `0' value means that the lattice site is not part of a blinker region.
	// A `1' value means that the lattice site is part of a blinker region.
	blinker_regions=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));

	// Initialize this to `0'.
	memset (blinker_regions, 0, sizeof(short int)*(p->xlen)*(p->ylen));

	// These two arrays are used to determine the two possible values
	// that the spins in each blinker region can take.
	blinker_region_values[0]=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
	blinker_region_values[1]=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));

	// Initialize these arrays to MAX_Q.
	// Since MAX_Q is not a valid value for any spin, MAX_Q signals
	// that a spin value for the blinker region has not yet been recorded.
	for (i=0; i<(p->xlen)*(p->ylen); i++)
	{
		*(blinker_region_values[0]+i)=MAX_Q;
		*(blinker_region_values[1]+i)=MAX_Q;
	}

	// Backup all simulation data.
	memcpy (spins_backup, p->spins, sizeof(short int)*(p->xlen)*(p->ylen));
	memcpy (state_backup, p->state, sizeof(short int)*(p->xlen)*(p->ylen));
	for (i=0; i<11+q*(q-1); i++)
	{
		memcpy (map_backup[i], p->map[i], sizeof(int)*(p->xlen)*(p->ylen));
		memcpy (index_backup[i], p->index[i], sizeof(int)*(1+(p->xlen)*(p->ylen)));
		prob_backup[i]=p->prob[i];
	}
	coordinate_backup=p->coordinate;
	move_backup=p->move;
	steps_backup=p->steps;
	time_backup=p->time;
	energy_backup=p->energy;
	mag_backup=p->mag;
	for (i=0; i<q; i++)
	{
		num_value_backup[i]=p->num_value[i];
	}
	memcpy (T_vertices_backup, p->T_vertices, sizeof(short int)*(p->xlen)*(p->ylen));
	memcpy (T_vertices_2_backup, p->T_vertices_2, sizeof(short int)*(p->xlen)*(p->ylen));

	for (bias=0; bias<2*q; bias++)
	{
		run_force=0;

		eligible_spins=0;

		for (i=0; i<q*(q-1); i++)
		{
			p->prob[11+i]=0;
		}

		if (bias%2==0)
		{
			for (i=0; i<q*(q-1); i++)
			{
				j=i/(q-1);
				k=((i%(q-1))+j+1)%q;

				if (j!=k && k==bias/2)
				{
					p->prob[11+i]=0.5;

					if (*(p->index[11+i])>0) eligible_spins++;
				}
			}

			if (eligible_spins>0) run_force=1;
		}
		else
		{
			for (i=0; i<q-1; i++)
			{
				p->prob[11+(bias/2)*(q-1)+i]=0.5;

				if (*(p->index[11+(bias/2)*(q-1)+i])>0) eligible_spins++;
			}

			if (eligible_spins>0) run_force=1;
		}

		// Check if there are eligible spins.
		if (run_force==1)
		{
			// Backup the state of the random number generator.
			rng_seed_backup=(p->rng_struct)->seed;
			rng_long_int_backup=(p->rng_struct)->current_long_int;
			rng_double_backup=(p->rng_struct)->current_double;
			(p->rng_struct)->rng_backup();
			
			// Drive the system with a bias field.
			if (run(	0,
					0,
					0,
					0,
					0,
					p,
					0)
				==result_error) return result_error;

			// Restore the state of the random number generator.
			(p->rng_struct)->seed=rng_seed_backup;
			(p->rng_struct)->current_long_int=rng_long_int_backup;
			(p->rng_struct)->current_double=rng_double_backup;
			(p->rng_struct)->rng_restore();
		}

		if (bias%2==0)
		{
			memcpy (spins_plus_field, p->spins, sizeof(short int)*(p->xlen)*(p->ylen));
		}
		else
		{
			memcpy (spins_minus_field, p->spins, sizeof(short int)*(p->xlen)*(p->ylen));

			for (y=0; y<(p->ylen); y++)
			{
				for (x=0; x<(p->xlen); x++)
				{
					if (*(spins_plus_field+x+y*(p->xlen))!=*(spins_minus_field+x+y*(p->xlen)))
					{
						*(blinker_regions+x+y*(p->xlen))=1;

						if (*(blinker_region_values[0]+x+y*(p->xlen))==MAX_Q) *(blinker_region_values[0]+x+y*(p->xlen))=bias/2;
						// In case there are pseudo-blinkers, only save the
						// first two possible spin types.
						else if (*(blinker_region_values[1]+x+y*(p->xlen))==MAX_Q) *(blinker_region_values[1]+x+y*(p->xlen))=bias/2;
					}
				}
			}
		}
	}
	
	// Restore the original lattice.
	memcpy (p->spins, spins_backup, sizeof(short int)*(p->xlen)*(p->ylen));
	memcpy (p->state, state_backup, sizeof(short int)*(p->xlen)*(p->ylen));
	for (i=0; i<11+q*(q-1); i++)
	{
		memcpy (p->map[i], map_backup[i], sizeof(int)*(p->xlen)*(p->ylen));
		memcpy (p->index[i], index_backup[i], sizeof(int)*(1+(p->xlen)*(p->ylen)));
		p->prob[i]=prob_backup[i];
	}
	p->coordinate=coordinate_backup;
	p->move=move_backup;
	p->steps=steps_backup;
	p->time=time_backup;
	p->energy=energy_backup;
	p->mag=mag_backup;
	for (i=0; i<q; i++)
	{
		num_value_backup[i]=p->num_value[i];
	}
	memcpy (p->T_vertices, T_vertices_backup, sizeof(short int)*(p->xlen)*(p->ylen));
	memcpy (p->T_vertices_2, T_vertices_2_backup, sizeof(short int)*(p->xlen)*(p->ylen));

// Plot the blinker regions.

	fprintf(matlab_data_1, "%d\n%d\n", p->xlen, p->ylen);
	
	for (y=0; y<(p->ylen); y++)
	{
		for (x=0; x<(p->xlen); x++)
		{
			fprintf(matlab_data_1, "%d\n", *(blinker_regions+x+y*(p->xlen)));
		}
	}
	
	fprintf(matlab_plot_1, "plotdata=fopen('%s', 'r');\n", matlab_data_filename_1);
	fprintf(matlab_plot_1, "\n");
	fprintf(matlab_plot_1, "nx=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot_1, "ny=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot_1, "\n");
	fprintf(matlab_plot_1, "h=zeros(nx, ny);\n");
	fprintf(matlab_plot_1, "\n");
	fprintf(matlab_plot_1, "figure\n");
	fprintf(matlab_plot_1, "axis('off', 'equal', [0 nx 0 ny])\n");
	fprintf(matlab_plot_1, "\n");
	fprintf(matlab_plot_1, "for y=0:ny-1\n");
	fprintf(matlab_plot_1, "	for x=0:nx-1\n");
	fprintf(matlab_plot_1, "		blinker_region=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot_1, "		if blinker_region==1\n");
	fprintf(matlab_plot_1, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'k', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_1, "		else\n");
	fprintf(matlab_plot_1, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'w', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_1, "		end\n");
	fprintf(matlab_plot_1, "	end\n");
	fprintf(matlab_plot_1, "end\n");

// Plot the value 1 of the blinker regions.

	fprintf(matlab_data_2, "%d\n%d\n", p->xlen, p->ylen);
	
	for (y=0; y<(p->ylen); y++)
	{
		for (x=0; x<(p->xlen); x++)
		{
			fprintf(matlab_data_2, "%d\n", *(blinker_region_values[0]+x+y*(p->xlen)));
		}
	}
	
	fprintf(matlab_plot_2, "plotdata=fopen('%s', 'r');\n", matlab_data_filename_2);
	fprintf(matlab_plot_2, "\n");
	fprintf(matlab_plot_2, "nx=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot_2, "ny=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot_2, "\n");
	fprintf(matlab_plot_2, "h=zeros(nx, ny);\n");
	fprintf(matlab_plot_2, "\n");
	fprintf(matlab_plot_2, "figure\n");
	fprintf(matlab_plot_2, "axis('off', 'equal', [0 nx 0 ny])\n");
	fprintf(matlab_plot_2, "\n");
	fprintf(matlab_plot_2, "for y=0:ny-1\n");
	fprintf(matlab_plot_2, "	for x=0:nx-1\n");
	fprintf(matlab_plot_2, "		spin=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot_2, "		if spin==0\n");
	fprintf(matlab_plot_2, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'r', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_2, "		elseif spin==1\n");
	fprintf(matlab_plot_2, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'g', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_2, "		elseif spin==2\n");
	fprintf(matlab_plot_2, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'b', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_2, "		elseif spin==3\n");
	fprintf(matlab_plot_2, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'y', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_2, "		elseif spin==4\n");
	fprintf(matlab_plot_2, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'm', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_2, "		elseif spin==5\n");
	fprintf(matlab_plot_2, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'c', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_2, "		elseif spin==6\n");
	fprintf(matlab_plot_2, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [1 0.55 0], 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_2, "		elseif spin==7\n");
	fprintf(matlab_plot_2, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [0.55 0.27 0.07], 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_2, "		elseif spin==8\n");
	fprintf(matlab_plot_2, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [0.5 0.5 0.5], 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_2, "		else\n");
	fprintf(matlab_plot_2, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'w', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_2, "		end\n");
	fprintf(matlab_plot_2, "	end\n");
	fprintf(matlab_plot_2, "end\n");

// Plot the value 2 of the blinker regions.

	fprintf(matlab_data_3, "%d\n%d\n", p->xlen, p->ylen);
	
	for (y=0; y<(p->ylen); y++)
	{
		for (x=0; x<(p->xlen); x++)
		{
			fprintf(matlab_data_3, "%d\n", *(blinker_region_values[1]+x+y*(p->xlen)));
		}
	}
	
	fprintf(matlab_plot_3, "plotdata=fopen('%s', 'r');\n", matlab_data_filename_3);
	fprintf(matlab_plot_3, "\n");
	fprintf(matlab_plot_3, "nx=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot_3, "ny=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot_3, "\n");
	fprintf(matlab_plot_3, "h=zeros(nx, ny);\n");
	fprintf(matlab_plot_3, "\n");
	fprintf(matlab_plot_3, "figure\n");
	fprintf(matlab_plot_3, "axis('off', 'equal', [0 nx 0 ny])\n");
	fprintf(matlab_plot_3, "\n");
	fprintf(matlab_plot_3, "for y=0:ny-1\n");
	fprintf(matlab_plot_3, "	for x=0:nx-1\n");
	fprintf(matlab_plot_3, "		spin=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot_3, "		if spin==0\n");
	fprintf(matlab_plot_3, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'r', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_3, "		elseif spin==1\n");
	fprintf(matlab_plot_3, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'g', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_3, "		elseif spin==2\n");
	fprintf(matlab_plot_3, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'b', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_3, "		elseif spin==3\n");
	fprintf(matlab_plot_3, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'y', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_3, "		elseif spin==4\n");
	fprintf(matlab_plot_3, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'm', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_3, "		elseif spin==5\n");
	fprintf(matlab_plot_3, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'c', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_3, "		elseif spin==6\n");
	fprintf(matlab_plot_3, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [1 0.55 0], 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_3, "		elseif spin==7\n");
	fprintf(matlab_plot_3, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [0.55 0.27 0.07], 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_3, "		elseif spin==8\n");
	fprintf(matlab_plot_3, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [0.5 0.5 0.5], 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_3, "		else\n");
	fprintf(matlab_plot_3, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'w', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot_3, "		end\n");
	fprintf(matlab_plot_3, "	end\n");
	fprintf(matlab_plot_3, "end\n");

	return;
}

void cluster_spins_plot(short int *cluster_spins, int nx, int ny, int nz, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot)
{
/*
	int x, y, z;
	int nxtimesny=nx*ny;
	
	fprintf(matlab_data, "%d\n%d\n%d\n", nx, ny, nz);
	
	for (z=0; z<nz; z++)
	{
		for (y=0; y<ny; y++)
		{
			for (x=0; x<nx; x++)
			{
				fprintf(matlab_data, "%d\n", *(cluster_spins+x+nx*y+nxtimesny*z));
			}
		}
	}
	
	fprintf(matlab_plot, "plotdata=fopen('%s', 'r');\n", matlab_data_filename);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "nx=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "ny=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "nz=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "hx=zeros(nx+1, ny, nz);\n");
	fprintf(matlab_plot, "hy=zeros(nx, ny+1, nz);\n");
	fprintf(matlab_plot, "hz=zeros(nx, ny, nz+1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "figure\n");
	fprintf(matlab_plot, "axis('off', 'equal', [0 nx 0 ny 0 nz])\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for z=0:nz-1\n");
	fprintf(matlab_plot, "	for y=0:ny-1\n");
	fprintf(matlab_plot, "		for x=0:nx-1\n");
	fprintf(matlab_plot, "			if fscanf(plotdata, '%%d', 1)==1\n");
	fprintf(matlab_plot, "				if hx(x+1, y+1, z+1)==0\n");
	fprintf(matlab_plot, "					hx(x+1, y+1, z+1)=patch([x x x x], [y y+1 y+1 y], [z z z+1 z+1], 'r');\n");
	fprintf(matlab_plot, "				end\n");
	fprintf(matlab_plot, "				if hx(x+2, y+1, z+1)==0\n");
	fprintf(matlab_plot, "					hx(x+2, y+1, z+1)=patch([x+1 x+1 x+1 x+1], [y y+1 y+1 y], [z z z+1 z+1], 'r');\n");
	fprintf(matlab_plot, "				end\n");
	fprintf(matlab_plot, "				if hy(x+1, y+1, z+1)==0\n");
	fprintf(matlab_plot, "					hy(x+1, y+1, z+1)=patch([x x x+1 x+1], [y y y y], [z z+1 z+1 z], 'g');\n");
	fprintf(matlab_plot, "				end\n");
	fprintf(matlab_plot, "				if hy(x+1, y+2, z+1)==0\n");
	fprintf(matlab_plot, "					hy(x+1, y+2, z+1)=patch([x x x+1 x+1], [y+1 y+1 y+1 y+1], [z z+1 z+1 z], 'g');\n");
	fprintf(matlab_plot, "				end\n");
	fprintf(matlab_plot, "				if hz(x+1, y+1, z+1)==0\n");
	fprintf(matlab_plot, "					hz(x+1, y+1, z+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [z z z z], 'b');\n");
	fprintf(matlab_plot, "				end\n");
	fprintf(matlab_plot, "				if hz(x+1, y+1, z+2)==0\n");
	fprintf(matlab_plot, "					hz(x+1, y+1, z+2)=patch([x x+1 x+1 x], [y y y+1 y+1], [z+1 z+1 z+1 z+1], 'b');\n");
	fprintf(matlab_plot, "				end\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "end\n");
*/	
	return;
}

void state_plot_matlab(short int *spins, int nx, int ny, int steps, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot)
{
	int x, y;
	int nxtimesny=nx*ny;
	
	fprintf(matlab_data, "%d\n%d\n%d\n", nx, ny, steps);
	
	for (y=0; y<ny; y++)
	{
		for (x=0; x<nx; x++)
		{
			fprintf(matlab_data, "%d\n", *(spins+x+nx*y));
		}
	}
	
	fprintf(matlab_plot, "plotdata=fopen('%s', 'r');\n", matlab_data_filename);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "nx=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "ny=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "steps=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "h=zeros(nx, ny);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "figure\n");
	fprintf(matlab_plot, "axis('off', 'equal', [0 nx 0 ny])\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for y=0:ny-1\n");
	fprintf(matlab_plot, "	for x=0:nx-1\n");
	fprintf(matlab_plot, "		spin=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "		if spin==0\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'r', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		elseif spin==1\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'g', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		elseif spin==2\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'b', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		elseif spin==3\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'y', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		elseif spin==4\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'm', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		elseif spin==5\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'c', 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		elseif spin==6\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [1 0.55 0], 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		elseif spin==7\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [0.55 0.27 0.07], 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [0.5 0.5 0.5], 'EdgeAlpha', 0);\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "end\n");

	return;
}
