package pratice.screenlock_v_1;

public class Point {
	public static int STATE_NORMAL = 1;
	public static int STATE_PRESSED= 2;
	public static int STATE_ERROR  = 3;
	public int state;
	float x;
	float y;
	public Point(float x, float y) {
		this.x = x;
		this.y = y;
		this.state = STATE_NORMAL;
	}
	
	public float getDistance(Point a) {
		return (float) Math.sqrt((x-a.x)*(x-a.x) + (y-a.y)*(y-a.y));
	}
}
