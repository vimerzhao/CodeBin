package zhaoyu1995.exception;

public class GameException extends RuntimeException {

	private static final long serialVersionUID = 1L;//根据警告添加

	public GameException(String s) {
		super(s);
	}
}
