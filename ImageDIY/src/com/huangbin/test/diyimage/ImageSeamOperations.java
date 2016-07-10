package com.huangbin.test.diyimage;
import android.graphics.Bitmap;

/*
 * This file is part of SeamCarving.
 *
 * SeamCarving is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * SeamCarving is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SeamCarving; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Copyright statement:
 * ********************
 * (c) 2007 by Mathias Lux (mathias@juggle.at)
 * Palmengasse 20, 8010 Graz, Austria
 * http://www.semanticmetadata.net
 */
public class ImageSeamOperations {
    private Bitmap img;
    private Bitmap energyAddon = null;
    public ImageSeamOperations(Bitmap img, Bitmap energyAddon) {
        this.img = img;
        this.energyAddon = energyAddon;     
    }
    public void setImage(Bitmap bmp){
    	img=bmp;
    }
    public ImageSeamOperations(Bitmap img) {
        this.img = img;
        
    }
    private boolean getRowPixels(Bitmap bmp,int row,int []pixels){
    	if(row<0||row>bmp.getHeight()-1)
    		return false;
    	for(int x=0;x<bmp.getWidth();x++)
    		pixels[x]=bmp.getPixel(x, row);
    	return true;
    }
    private boolean setRowPixels(Bitmap bmp,int row,int []pixels){
    	if(row<0||row>bmp.getHeight()-1)
    		return false;
    	for(int x=0;x<bmp.getWidth();x++)
    		bmp.setPixel(x, row, pixels[x]);
    	return true;
    }
    private boolean getColPixels(Bitmap bmp,int col,int []pixels){
    	if(col<0||col>bmp.getWidth()-1)
    		return false;
    	for(int y=0;y<bmp.getHeight();y++)
    		pixels[y]=bmp.getPixel(col,y);
    	return true;
    }
   /* private boolean setColPixels(Bitmap bmp,int col,int []pixels){
    	if(col<0||col>bmp.getWidth()-1)
    		return false;
    	for(int y=0;y<bmp.getHeight();y++)
    		bmp.setPixel(col, y, pixels[y]);
    	return true;
    }*/
    public VerticalSeam findOptimalVerticalSeam() {
        Bitmap energyImg = generateEnergyImage();
        float[][][] table = createVerticalSeamTable(energyImg);
        float energy = Float.MAX_VALUE;
        int seamstart = 0;
        int height = energyImg.getHeight();
        for (int i = 1; i < table.length - 1; i++) {
            if (table[i][height - 1][0] < energy) {
                energy = table[i][height - 1][0];
                seamstart = i;
            }
        }
        return new VerticalSeam(energy, seamstart, table);
    }

    public HorizontalSeam findOptimalHorizontalSeam() {
        Bitmap energyImg = generateEnergyImage();
        float[][][] table = createHorizontalSeamTable(energyImg);
     /*   try{
        	printSeamTable(table);
        }
        catch(IOException e){
        	
        }*/
        // find the optimal seam.
        float energy = img.getHeight() * 255;
        int seamstart = 0;
        int width = energyImg.getWidth();
        for (int i = 1; i < table[0].length - 1; i++) {
            if (table[width - 1][i][0] < energy) {
                energy = table[width - 1][i][0];
                seamstart = i;
            }
        }
        return new HorizontalSeam(energy, seamstart, table);
    }

    private float[][][] createHorizontalSeamTable(Bitmap energyImage) {
      
        int pixelCol[] = new int[energyImage.getHeight()];
        float[][][] result = new float[energyImage.getWidth()][energyImage.getHeight()][2];
        int h = energyImage.getHeight();
        int w = energyImage.getWidth();
        getColPixels(energyImage, 0, pixelCol);
        // init first col of pixels
        for (int y = 0; y < h; y++) {
            result[0][y][0] = pixelCol[y]&0xff;
        }
        // now for the following rows find the next component in the seam
        float right, left, min, middle, currentEnergy;
        int nextStep;
        for (int x = 1; x < w; x++) {
            
        	getColPixels(energyImage, x, pixelCol);
            for (int y = 0; y < h; y++) {
                // find 'next in path'
                middle = result[x - 1][y][0];
                currentEnergy = pixelCol[y]&0xff;
                nextStep = 0;
                if (y > 1 && y < h - 2) {
                    left = result[x - 1][y - 1][0];
                    right = result[x - 1][y + 1][0];
                    nextStep = 0;
                    if (right < middle || left < middle) {
                        if (right <= left) {
                            min = right;
                            nextStep = 1;
                        } else {
                            min = left;
                            nextStep = -1;
                        }
                    } else {
                        min = middle;
                    }
                    currentEnergy = currentEnergy + min;
                } else if (y > 1) { // left border of the image, no left possible
                    right = result[x - 1][y - 1][0];
                    if (right < middle) {
                        min = right;
                        nextStep = -1;
                    } else {
                        min = middle;
                        nextStep = 0;
                    }
                    currentEnergy = currentEnergy + min;
                } else if (y < h - 2) { // right border of the image, no right possible
                    left = result[x - 1][y + 1][0];
                    if (left < middle) {
                        min = left;
                        nextStep = 1;
                    } else {
                        min = middle;
                        nextStep = 0;
                    }
                    currentEnergy = currentEnergy + min;
                }
                result[x][y][0] = currentEnergy;
                result[x][y][1] = nextStep;
            }
        }
        return result;
    }

    public void carveVerticalSeam() {
        VerticalSeam s = findOptimalVerticalSeam();
        int height = img.getHeight();
        // lookup table for the seam coordinates: y -> x as result of the backtracking.
        int[] seamLookup = new int[height];
        // backtracking:
        int nextX = s.startX;
        seamLookup[height - 1] = nextX;
        for (int y = height - 2; y >= 0; y--) {
            nextX += (int) s.table[nextX][y][1];
            seamLookup[y] = nextX;
        }
        Bitmap target=Bitmap.createBitmap(img.getWidth()-1,img.getHeight(),Bitmap.Config.ARGB_8888);
        int[] row = new int[img.getWidth()];
        int[] newRow = new int[target.getWidth()];
        for (int y = 0; y < img.getHeight(); y++) {
            
        	getRowPixels(img,y,row);
            int skipX = seamLookup[y];
            System.arraycopy(row, 0, newRow, 0, skipX);
            System.arraycopy(row, (skipX + 1), newRow, skipX, (target.getWidth() - skipX - 1));        
            setRowPixels(target,y,newRow);
        }
        img = target;

        if (energyAddon!=null) {
        	  Bitmap energytarget=Bitmap.createBitmap(energyAddon.getWidth()-1,energyAddon.getHeight(),Bitmap.Config.ARGB_8888);         
            row = new int[energytarget.getWidth()];
            newRow = new int[(energytarget.getWidth() - 1)];
            for (int y = 0; y < energyAddon.getHeight(); y++) {             
            	getRowPixels(energyAddon,y,row);
                int skipX = seamLookup[y];
                System.arraycopy(row, 0, newRow, 0, skipX);
                System.arraycopy(row, (skipX + 1), newRow, skipX, (target.getWidth() - skipX - 1));             
                setRowPixels(energytarget,y,newRow);
            }
            energyAddon = target;
        }
    }

    public void carveHorizontalSeam() {
        HorizontalSeam s = findOptimalHorizontalSeam();
        int height = img.getHeight();
        int width = img.getWidth();
        // lookup table for the seam coordinates: x -> y as result of the backtracking.
        int[] seamLookup = new int[width];
        // backtracking:
        int nextY = s.startY;
        seamLookup[width - 1] = nextY;
        for (int x = width - 2; x >= 0; x--) {
            nextY += (int) s.table[x][nextY][1];
            seamLookup[x] = nextY;
        }
        Bitmap target=Bitmap.createBitmap(width,height-1,Bitmap.Config.ARGB_8888); 
        //int[] pixel = new int[1];
        int pixel;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                pixel=img.getPixel(x, y);
                int targetY = y;
                if (seamLookup[x] != y) {
                    if (seamLookup[x] <= y) {
                        targetY--;
                    }
                    target.setPixel(x, targetY, pixel);
                }
            }
        }
        img = target;
        if (energyAddon!=null) {
        	int Ewidth=energyAddon.getWidth();
        	int Eheight=energyAddon.getHeight();
            Bitmap energytarget=Bitmap.createBitmap(Ewidth,Eheight-1,Bitmap.Config.ARGB_8888);                   
            int energypixel;
            for (int y = 0; y < Eheight; y++) {
                for (int x = 0; x < Ewidth; x++) {
                   energypixel= energyAddon.getPixel(x, y);
                    int targetY = y;
                    if (seamLookup[x] != y) {
                        if (seamLookup[x] <= y) {
                            targetY--;
                        }
                        energytarget.setPixel(x, targetY, energypixel);
                    }
                }
            }
            energyAddon = energytarget;
        }
    }

   /* private void printSeamTable(float[][][] table) throws IOException {
        BufferedWriter bw = new BufferedWriter(new FileWriter("/sdcard/table.txt"));
        for (int i = 0; i < img.getWidth(); i++) {
            for (int j = 0; j < img.getHeight(); j++) {
                bw.write((int) table[i][j][0] + " -> " + (int) table[i][j][1] + "\t");
            }
            bw.write('\n');
        }
        bw.close();
    }
*/
    /**
     * Generates a table containing all possible seams using dynamic programming.
     * To find a seam start at the last row and use backtracking: y -> y-1, x -> x+ nextX
     *
     * @param energyImage the greyscale image containing the energy per pixel.
     * @return the seam table of the dynamic programming continaing [x][y][0:energy, 1:nextX]
     */
    private float[][][] createVerticalSeamTable(Bitmap energyImage) {
    	int h = energyImage.getHeight();
        int w = energyImage.getWidth();
    	int pixelRow[] = new int[w];
        float[][][] result = new float[w][h][2];
        getRowPixels(energyImage,0,pixelRow);
        for (int x = 0; x < w; x++) {
            result[x][0][0] = pixelRow[x]&0xff;
        }
        float right, left, min, middle, currentEnergy;
        int nextStep;
        for (int y = 1; y < h; y++) {
        	getRowPixels(energyImage, y, pixelRow);
            for (int x = 0; x < w; x++) {
                middle = result[x][y - 1][0];
                currentEnergy = pixelRow[x]&0xff;
                nextStep = 0;
                if (x > 1 && x < w - 2) {
                    left = result[x - 1][y - 1][0];
                    right = result[x + 1][y - 1][0];
                    nextStep = 0;
                    if (right < middle || left < middle) {
                        if (right < left) {
                            min = right;
                            nextStep = 1;
                        } else {
                            min = left;
                            nextStep = -1;
                        }
                    } else {
                        min = middle;
                    }
                    currentEnergy = currentEnergy + min;
                } else if (x > 1) { // left border of the image, no left possible
                    right = result[x - 1][y - 1][0];
                    if (right < middle) {
                        min = right;
                        nextStep = -1;
                    } else {
                        min = middle;
                        nextStep = 0;
                    }
                    currentEnergy = currentEnergy + min;
                } else if (x < w - 2) { // right border of the image, no right possible
                    left = result[x + 1][y - 1][0];
                    if (left < middle) {
                        min = left;
                        nextStep = 1;
                    } else {
                        min = middle;
                        nextStep = 0;
                    }
                    currentEnergy = currentEnergy + min;
                }
                result[x][y][0] = currentEnergy;
                result[x][y][1] = nextStep;
            }
        }
        return result;
    }

    /**
     * This method generates the energy image. Change this one to use another one besides the gradient.
     *
     * @return a grayscale image denoting the energy of the pixels.
     */
    private Bitmap generateEnergyImage() {
        // convert image to greyscale:
        Bitmap img_grey = ImageUtils.GrayImage(img);
        Bitmap grad01 = ImageUtils.bianYuan(img_grey);
        int temp;
        if (energyAddon!=null) {
            	   for (int x = 0; x < grad01.getWidth(); x++) {
                       for (int y = 0; y < grad01.getHeight(); y++) {
                    if ((energyAddon.getPixel(x,y)&0xff)>0) {
                        temp= 0xffffffff;
                        grad01.setPixel(x,y,temp);
                    } 
                }
                
            }
        }
        return grad01;
    }

    public Bitmap getImg() {
        return img;
    }
}

class VerticalSeam { //implements Comparable {
//class VerticalSeam{
    public float energy;
    public int startX;
    public float[][][] table;


    public VerticalSeam(float energy, int startX, float[][][] table) {
        this.energy = energy;
        this.startX = startX;
        this.table = table;
    }

  /*  public int compareTo(Object o) {
        int result = 0;
        if (o instanceof VerticalSeam) {
            VerticalSeam s = (VerticalSeam) o;
            float e = s.energy - energy;
            result = (int) Math.signum(e);
        }
        return result;
    }*/
}

class HorizontalSeam {//implements Comparable {
    public float energy;
    public int startY;
    public float[][][] table;


    public HorizontalSeam(float energy, int startY, float[][][] table) {
        this.energy = energy;
        this.startY = startY;
        this.table = table;
    }

   /* public int compareTo(Object o) {
        int result = 0;
        if (o instanceof HorizontalSeam) {
            HorizontalSeam s = (HorizontalSeam) o;
            float e = s.energy - energy;
            result = (int) Math.signum(e);
        }
        return result;
    }*/
}
