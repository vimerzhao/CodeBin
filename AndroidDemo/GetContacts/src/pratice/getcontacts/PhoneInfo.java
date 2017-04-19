package pratice.getcontacts;
//·â×°ÐÅÏ¢
public class PhoneInfo {
	String name;
	String number;
	
	public PhoneInfo(String name, String number){
		setName(name);
		setNumber(number);
	}
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getNumber() {
		return number;
	}
	public void setNumber(String number) {
		this.number = number;
	}
	
	
}
