/*** Silicon Calibration ***/
//y = A * x + B
	A=0;B=0;
	x=0;y=0;
	n=0;
	
	if(ElossMSX1>0 && ElossMSX1<0.0075 && ElossBB2>0.0023){
	cnt = cnt + 1;
	if (cnt>10 && cnt<100){
	n = cnt;
	x = *msx[0];
	y = ElossMSX1 * 1000; //[MeV]
	//x = *msx[1];
	//y = ElossMSX2 * 1000;
	sum_x += x; 
	sum_y += y;
	sum_x_squared += pow(x,2);
	sum_xy += x*y;
	
	A = (n*sum_xy - sum_x*sum_y)/(n*sum_x_squared - pow(sum_x,2));
	B = (sum_y*sum_x_squared - sum_x*sum_xy)/(n*sum_x_squared - pow(sum_x,2));
	
	std::cout<<"A="<<A<<",  B="<<B<<std::endl;
	}
	}

//-------------------------------
    
