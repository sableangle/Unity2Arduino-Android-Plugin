using UnityEngine;
using System.Collections;

public class Test : MonoBehaviour {
	public int ScreenHeight = 1280;
	public int ScreenWidth = 720;
	float h;
	float w;
	private TextMesh ThreadMesh;
	private TextMesh ServiceMesh;
	void Awake(){
		w = (float)Screen.width / ScreenWidth;
		h = (float)Screen.height / ScreenHeight;
	}
	// Use this for initialization
	void Start () {
		ThreadMesh = GameObject.Find ("Thread").GetComponent<TextMesh>();
		ServiceMesh = GameObject.Find ("Service").GetComponent<TextMesh>();
	}
	void MsgFromAndroidThread(string a){
		ThreadMesh.text = a;
	}
	void MsgFromAndroidService(string a){
		ServiceMesh.text = a;
	}
	// Update is called once per frame
	void Update () {

	}
	void OnGUI(){
		Matrix4x4 _matrix = GUI.matrix;
		_matrix.m00 = w;
		_matrix.m11 = h;
		GUI.matrix = _matrix;
		
		if (GUI.Button(new Rect(50, 50, 200, 50), "Toast")){
			AndroidJavaClass jc=new AndroidJavaClass("com.unity3d.player.UnityPlayer");  
			AndroidJavaObject jo=jc.GetStatic<AndroidJavaObject>("currentActivity");  
			jo.Call("CallToast","Send From Unity"); 
		}
		if (GUI.Button(new Rect(300, 50, 200, 50), "Vibrator")){
			AndroidJavaClass unityPlayer = new AndroidJavaClass("com.unity3d.player.UnityPlayer");
			AndroidJavaObject currentActivity = unityPlayer.GetStatic<AndroidJavaObject>("currentActivity");
			AndroidJavaObject vibrator = currentActivity.Call<AndroidJavaObject>("getSystemService", "vibrator"); 
			vibrator.Call("vibrate",1000);
		}

		if (GUI.Button(new Rect(50, 150, 200, 50), "BlueOn")){
			AndroidJavaClass jc=new AndroidJavaClass("com.unity3d.player.UnityPlayer");  
			AndroidJavaObject jo=jc.GetStatic<AndroidJavaObject>("currentActivity");  
			jo.Call("SetBluedeviceOn");  
		}
		if (GUI.Button(new Rect(300, 150, 200, 50), "BlueOff")){
			AndroidJavaClass jc=new AndroidJavaClass("com.unity3d.player.UnityPlayer");  
			AndroidJavaObject jo=jc.GetStatic<AndroidJavaObject>("currentActivity");  
			jo.Call("SetBluedeviceOff");  
		}
		if (GUI.Button(new Rect(300, 250, 200, 50), "Diglobox")){
			AndroidJavaClass jc=new AndroidJavaClass("com.unity3d.player.UnityPlayer");  
			AndroidJavaObject jo=jc.GetStatic<AndroidJavaObject>("currentActivity");  
			string[] obj = new string[2];
			obj[0] = "abc";
			obj[1] = "edgdfas";
			jo.Call("ShowDialog",obj);  
		}
		if (GUI.Button(new Rect(50, 250, 200, 50), "Discoverable")){
			AndroidJavaClass jc=new AndroidJavaClass("com.unity3d.player.UnityPlayer");  
			AndroidJavaObject jo=jc.GetStatic<AndroidJavaObject>("currentActivity");  
			jo.Call("SetDiscoverable");  
		}
		if (GUI.Button(new Rect(50, 350, 200, 50), "Open The Scan Panel")){
			AndroidJavaClass jc=new AndroidJavaClass("com.unity3d.player.UnityPlayer");  
			AndroidJavaObject jo=jc.GetStatic<AndroidJavaObject>("currentActivity");  
			jo.Call("OpenScanPanel");  
		}
		if (GUI.Button(new Rect(300, 350, 200, 50), "ChangePage")){
			AndroidJavaClass jc=new AndroidJavaClass("com.unity3d.player.UnityPlayer");  
			AndroidJavaObject jo=jc.GetStatic<AndroidJavaObject>("currentActivity");  
			jo.Call("StopSeverAndThread");  
		}
		if (GUI.Button(new Rect(50, 450, 200, 50), "Start Chat")){
			AndroidJavaClass jc=new AndroidJavaClass("com.unity3d.player.UnityPlayer");  
			AndroidJavaObject jo=jc.GetStatic<AndroidJavaObject>("currentActivity");  
			jo.Call("setupChat");  
		}
	}
}
