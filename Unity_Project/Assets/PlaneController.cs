using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class PlaneController : MonoBehaviour {
	//GUI
	float h;
	float w;
	public int ScreenHeight = 720;
	public int ScreenWidth = 1280;
	//
	Scrollbar p1;
	Scrollbar p2;
	Scrollbar p3;
	Scrollbar p4;

	Scrollbar MainPower;
	Scrollbar Kp;

	Button StartBtn;
	Text StartBtnText;
	//
	bool start = false;

	//Bt output

	string outputData;
	int A1Speed;
	int A2Speed;
	int A3Speed;
	int A4Speed;

	int adjSpeed1;
	int adjSpeed2;
	int adjSpeed3;
	int adjSpeed4;

	int baseSpeed;
	int KpValue;


	//java
	AndroidJavaClass jc;
	AndroidJavaObject jo;

	void Awake(){
		w = (float)Screen.width / ScreenWidth;
		h = (float)Screen.height / ScreenHeight;
	}

	// Use this for initialization
	void Start () {
		p1 = GameObject.Find ("A1").GetComponent<Scrollbar>();
		p2 = GameObject.Find ("A2").GetComponent<Scrollbar>();
		p3 = GameObject.Find ("A3").GetComponent<Scrollbar>();
		p4 = GameObject.Find ("A4").GetComponent<Scrollbar>();
		MainPower = GameObject.Find ("Main").GetComponent<Scrollbar>();
		Kp = GameObject.Find ("Kp").GetComponent<Scrollbar>();

		StartBtn = GameObject.Find ("StartBtn").GetComponent<Button>();
		StartBtnText = StartBtn.GetComponentInChildren<Text>();

		jc=new AndroidJavaClass("com.unity3d.player.UnityPlayer");  
		jo=jc.GetStatic<AndroidJavaObject>("currentActivity");  
	}
	
	// Update is called once per frame
	void FixedUpdate () {
		if (start) {
			baseSpeed = (int)(MainPower.value * 10);
			KpValue = (int)(Kp.value * 10);
			/*
			adjSpeed1 = (int)(p1.value * 9);
			adjSpeed2 = (int)(p2.value * 9);
			adjSpeed3 = (int)(p3.value * 9);
			adjSpeed4 = (int)(p4.value * 9);

			outputData = "@"+ baseSpeed +"0000" + adjSpeed1 + adjSpeed2 + adjSpeed3 + adjSpeed4;
			*/
			outputData = "f,"+baseSpeed+","+KpValue+",";
			jo.Call("SendBluetoothMsgForUnity",outputData+'\n'); 
		}
		else{

		}
	}

	public void stopAll(){

		outputData = "@000000000";
		jo.Call("SendBluetoothMsgForUnity",outputData+'\n'); 

	}

	public void ChangeState(){
		if (start) {
			outputData = "b,";
			jo.Call("SendBluetoothMsgForUnity",outputData+'\n'); 		
		}
		start = !start;

		StartBtnText.text = start.ToString ();
	}

	public void StartScan(){
		jo.Call("OpenScanPanel","00001101-0000-1000-8000-00805f9b34fb"); 
	}

	void OnGUI(){
		Matrix4x4 _matrix = GUI.matrix;
		_matrix.m00 = w;
		_matrix.m11 = h;
		GUI.matrix = _matrix;
	}
}
