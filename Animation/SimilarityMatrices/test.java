package motionGraph;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.math.*;

import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.imgproc.Imgproc;

public class test {
	public static void main(String[] args) throws FileNotFoundException {
		//1: draw picture based on position, 2: draw picture based on rotation
		drawPicture(1);
		drawPicture(2);
	}
	
	public static void drawPicture(int caseType) throws FileNotFoundException {
		frame[] frameArray = getFrameArrayFromFile();
		float[][] distanceMatrix = getDistanceMatrix(frameArray,caseType);
		System.loadLibrary(Core.NATIVE_LIBRARY_NAME); 	
		Mat mat = new Mat(frameArray.length, frameArray.length, CvType.CV_8UC1);	
		
		//the next for loops is to limit the distance range to 0-255 so that it can match the color range.
		float maxDistance = 0;
		for(int i=0; i<distanceMatrix.length; i++) {
			for(int j=0; j<distanceMatrix[i].length; j++) {
				if(maxDistance < distanceMatrix[i][j]) {
					maxDistance = distanceMatrix[i][j];
				}
			}
		}				
		for(int i=0; i<distanceMatrix.length; i++) {
			for(int j=0; j<distanceMatrix[i].length; j++) {
				//the large distance will be drawn in black color while the small distance in white color.
				mat.put(i, j, 255-255*distanceMatrix[i][j]/maxDistance);
			}
		}
		if(caseType == 1) {
			Imgcodecs.imwrite("position.jpg", mat);  	
		} else {
			Imgcodecs.imwrite("rotation.jpg", mat);  	
		}
	}
	
	//get distance matrix
	public static float[][] getDistanceMatrix(frame[] frameArray, int caseType) {
		float[][] distanceMatrix = new float[frameArray.length][frameArray.length];	
		for(int i=0; i<distanceMatrix.length; i++) {
			for(int j=0; j<distanceMatrix[i].length; j++) {
				distanceMatrix[i][j] = calculateDistanceBetweenTwoFrames(frameArray[i],frameArray[j],caseType);
			}
		}		
		return distanceMatrix;
	}
	
	//calculate distance between two frames
	public static float calculateDistanceBetweenTwoFrames(frame f1, frame f2, int caseType) {
		float distance = 0;
		if(caseType == 1) {
			distance = calculatePositionBetweenTwoFrames(f1,f2);
		} else {
		    distance = calculateRotationBetweenTwoFrames(f1,f2);
		}
		return distance;
	}
	
	public static float calculatePositionBetweenTwoFrames(frame f1, frame f2) {
		float positionValue = 0;
		for(int i=0; i<3; i++) {
			positionValue += Math.pow((f1.getHeadTranslate()[i] - f2.getHeadTranslate()[i]), 2);		
			positionValue += Math.pow((f1.getChestTranslate()[i] - f2.getChestTranslate()[i]), 2);
			positionValue += Math.pow((f1.getRShoulderTranslate()[i] - f2.getRShoulderTranslate()[i]), 2);
			positionValue += Math.pow((f1.getLShoulderTranslate()[i] - f2.getLShoulderTranslate()[i]), 2);
			positionValue += Math.pow((f1.getRHandTranslate()[i] - f2.getRHandTranslate()[i]), 2);
			positionValue += Math.pow((f1.getLHandTranslate()[i] - f2.getLHandTranslate()[i]), 2);
			positionValue += Math.pow((f1.getRFootTranslate()[i] - f2.getRFootTranslate()[i]), 2);
			positionValue += Math.pow((f1.getLFootTranslate()[i] - f2.getLFootTranslate()[i]), 2);	
			
		}
		return positionValue;
	}
	
	public static float calculateRotationBetweenTwoFrames(frame f1, frame f2) {
		float rotationValue = 0;
		for(int i=0; i<3; i++) { 
			rotationValue += Math.pow((f1.getHeadRotate()[i] - f2.getHeadRotate()[i]),2);
			rotationValue += Math.pow((f1.getChestRotate()[i] - f2.getChestRotate()[i]),2);
			rotationValue += Math.pow((f1.getRShoulderRotate()[i] - f2.getRShoulderRotate()[i]),2);
			rotationValue += Math.pow((f1.getLShoulderRotate()[i] - f2.getLShoulderRotate()[i]),2);
			rotationValue += Math.pow((f1.getRHandRotate()[i] - f2.getRHandRotate()[i]),2);
			rotationValue += Math.pow((f1.getLHandRotate()[i] - f2.getLHandRotate()[i]),2);
			rotationValue += Math.pow((f1.getRFootRotate()[i] - f2.getRFootRotate()[i]),2);
			rotationValue += Math.pow((f1.getLFootRotate()[i] - f2.getLFootRotate()[i]),2);
		}	
		//the minimal distance is zero.
		if(Math.log(rotationValue) < 0)
			return 0;
		return (float) Math.log(rotationValue);
	}
	
	
	//get each frame information from file and write them into a frame array
	public static frame[] getFrameArrayFromFile() throws FileNotFoundException {
		Scanner scanner = new Scanner(new FileInputStream("markers.td"));
		frame[] frameArray = new frame[669];
		int i = 0;
		//first get rid of the head information
		while(scanner.hasNext() && i < 49) {
			String word = scanner.next();
			i++;
		}	
		
		//read each frame's information from file and write it in a frameArray
		i = 0;		
		while(scanner.hasNext()) {
			frame newFrame = new frame();
			float[] tempData = new float[3];
			newFrame.setFrameNumber(Integer.parseInt(scanner.next()));
			for(int j=0; j<3; j++) {
				tempData[j] = Float.parseFloat(scanner.next());
			}
			newFrame.setHeadTranslate(tempData);
			for(int j=0; j<3; j++) {
				tempData[j] = Float.parseFloat(scanner.next());
			}
			newFrame.setHeadRotate(tempData);

			for(int j=0; j<3; j++) {
				tempData[j] = Float.parseFloat(scanner.next());
			}
			newFrame.setChestTranslate(tempData);
			for(int j=0; j<3; j++) {
				tempData[j] = Float.parseFloat(scanner.next());
			}
			newFrame.setChestRotate(tempData);

			for(int j=0; j<3; j++) {
				tempData[j] = Float.parseFloat(scanner.next());
			}
			newFrame.setRShoulderTranslate(tempData);
			for(int j=0; j<3; j++) {
				tempData[j] = Float.parseFloat(scanner.next());
			}
			newFrame.setRShoulderRotate(tempData);
			
			for(int j=0; j<3; j++) {
				tempData[j] = Float.parseFloat(scanner.next());
			}
			newFrame.setLShoulderTranslate(tempData);
			for(int j=0; j<3; j++) {
				tempData[j] = Float.parseFloat(scanner.next());
			}
			newFrame.setLShoulderRotate(tempData);			
			
			for(int j=0; j<3; j++) {
				tempData[j] = Float.parseFloat(scanner.next());
			}
			newFrame.setRHandTranslate(tempData);
			for(int j=0; j<3; j++) {
				tempData[j] = Float.parseFloat(scanner.next());
			}
			newFrame.setRHandRotate(tempData);
		
			for(int j=0; j<3; j++) {
				tempData[j] = Float.parseFloat(scanner.next());
			}
			newFrame.setLHandTranslate(tempData);
			for(int j=0; j<3; j++) {
				tempData[j] = Float.parseFloat(scanner.next());
			}
			newFrame.setLHandRotate(tempData);
			
			for(int j=0; j<3; j++) {
				tempData[j] = Float.parseFloat(scanner.next());
			}
			newFrame.setRFootTranslate(tempData);
			for(int j=0; j<3; j++) {
				tempData[j] = Float.parseFloat(scanner.next());
			}
			newFrame.setRFootRotate(tempData);
			
			for(int j=0; j<3; j++) {
				tempData[j] = Float.parseFloat(scanner.next());
			}
			newFrame.setLFootTranslate(tempData);
			for(int j=0; j<3; j++) {
				tempData[j] = Float.parseFloat(scanner.next());
			}
			newFrame.setLFootRotate(tempData);		
			frameArray[i] = newFrame;
			i++;
		}
		return frameArray;
	}
	
}


//construct a frame class to store each frame's information.
class frame {
	public int getFrameNumber() {
		return frameNumber;
	}
	
	public void setFrameNumber(int frameNumber) {
		this.frameNumber = frameNumber;
	}
	
	public float[] getHeadTranslate() {
		return headTranslate;
	}
	
	public void setHeadTranslate(float[] headTranslate) {
		for(int i=0; i<3; i++) {
			this.headTranslate[i] = headTranslate[i];
		}
	}
	
	public float[] getHeadRotate() {
		return headRotate;
	}
	
	public void setHeadRotate(float[] headRotate) {
		for(int i=0; i<3; i++) {
			this.headRotate[i] = headRotate[i];
		}
	}
	
	public float[] getChestTranslate() {
		return chestTranslate;
	}
	
	public void setChestTranslate(float[] chestTranslate) {
		for(int i=0; i<3; i++) {
			this.chestTranslate[i] = chestTranslate[i];
		}
	}
	
	public float[] getChestRotate() {
		return chestRotate;
	}
	
	public void setChestRotate(float[] chestRotate) {
		for(int i=0; i<3; i++) {
			this.chestRotate[i] = chestRotate[i];
		}
	}
	
	public float[] getRShoulderTranslate() {
		return rShoulderTranslate;
	}
	
	public void setRShoulderTranslate(float[] rShoulderTranslate) {
		for(int i=0; i<3; i++) {
			this.rShoulderTranslate[i] = rShoulderTranslate[i];
		}
	}
	
	public float[] getRShoulderRotate() {
		return rShoulderRotate;
	}
	
	public void setRShoulderRotate(float[] rShoulderRotate) {
		for(int i=0; i<3; i++) {
			this.rShoulderRotate[i] = rShoulderRotate[i];
		}
	}
	
	public float[] getLShoulderTranslate() {
		return lShoulderTranslate;
	}
	
	public void setLShoulderTranslate(float[] lShoulderTranslate) {
		for(int i=0; i<3; i++) {
			this.lShoulderTranslate[i] = lShoulderTranslate[i];
		}
	}
	
	public float[] getLShoulderRotate() {
		return lShoulderRotate;
	}
	
	public void setLShoulderRotate(float[] lShoulderRotate) {
		for(int i=0; i<3; i++) {
			this.lShoulderRotate[i] = lShoulderRotate[i];
		}
	}
	
	public float[] getRHandTranslate() {
		return rHandTranslate;
	}
	
	public void setRHandTranslate(float[] rHandTranslate) {
		for(int i=0; i<3; i++) {
			this.rHandTranslate[i] = rHandTranslate[i];
		}
	}
	
	public float[] getRHandRotate() {
		return rHandRotate;
	}
	
	public void setRHandRotate(float[] rHandRotate) {
		for(int i=0; i<3; i++) {
			this.rHandRotate[i] = rHandRotate[i];
		}
	}
	
	public float[] getLHandTranslate() {
		return lHandTranslate;
	}
	
	public void setLHandTranslate(float[] lHandTranslate) {
		for(int i=0; i<3; i++) {
			this.lHandTranslate[i] = lHandTranslate[i];
		}
	}
	
	public float[] getLHandRotate() {
		return lHandRotate;
	}
	
	public void setLHandRotate(float[] lHandRotate) {
		for(int i=0; i<3; i++) {
			this.lHandRotate[i] = lHandRotate[i];
		}
	}
	
	public float[] getRFootTranslate() {
		return rFootTranslate;
	}
	
	public void setRFootTranslate(float[] rFootTranslate) {
		for(int i=0; i<3; i++) {
			this.rFootTranslate[i] = rFootTranslate[i];
		}
	}
	
	public float[] getRFootRotate() {
		return rFootRotate;
	}
	
	public void setRFootRotate(float[] rFootRotate) {
		for(int i=0; i<3; i++) {
			this.rFootRotate[i] = rFootRotate[i];
		}
	}
	
	public float[] getLFootTranslate() {
		return lFootTranslate;
	}
	
	public void setLFootTranslate(float[] lFootTranslate) {
		for(int i=0; i<3; i++) {
			this.lFootTranslate[i] = lFootTranslate[i];
		}
	}
	
	public float[] getLFootRotate() {
		return lFootRotate;
	}
	
	public void setLFootRotate(float[] lFootRotate) {
		for(int i=0; i<3; i++) {
			this.lFootRotate[i] = lFootRotate[i];
		}
	}
		
	private int frameNumber;
	private float[] headTranslate = new float[3];
	private float[] headRotate = new float[3];
	private float[] chestTranslate = new float[3];
	private float[] chestRotate = new float[3];
	private float[] rShoulderTranslate = new float[3];
	private float[] rShoulderRotate = new float[3];	
	private float[] lShoulderTranslate = new float[3];
	private float[] lShoulderRotate = new float[3];	
	private float[] rHandTranslate = new float[3];
	private float[] rHandRotate = new float[3];		
	private float[] lHandTranslate = new float[3];
	private float[] lHandRotate = new float[3];	
	private float[] rFootTranslate = new float[3];
	private float[] rFootRotate = new float[3];	
	private float[] lFootTranslate = new float[3];
	private float[] lFootRotate = new float[3];	
	
}