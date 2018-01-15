/************************************************************
*file:frame.js
*brief:Test results into a frame
*author:chen
*time:2018/01/04
************************************************************/
$(function() {
	var vlcH= $("#vlc").height();
	// vlc的宽高比为16:9,获取实际画面宽度
	var vlcW= vlcH*16/9;
	// 摄像头分辨率1280*720,宽高缩放系数
	var Ratio= vlcW/1280;

	var websocket = null;
	// 获取canvas
	var d=document.querySelector("#drawing");
	// 获取实际的vlc画面大小
	var vlcH= $(".camera_width").height();
	// vlc的宽高比为16:9
	var vlcW= vlcH*16/9;
	// 摄像头分辨率1280*720
	// 宽高缩放系数
	var ratio= vlcW/1280;
	// 轮询相机编号
	let poll_num="";
	// canvas画框相关
	d.width= vlcW;
	d.height= vlcH;
	// 确定浏览器支持canvas
	if(d.getContext) {
		var ctx= d.getContext("2d");
	}		
	// canvas函数
	function rect(x,y,w,h){
		ctx.lineWidth= 3;
		ctx.strokeStyle= "#f32222"; 	
		ctx.strokeRect(x*ratio,y*ratio,w*ratio,h*ratio);
	}
	function text(text,x,y) {
		ctx.textBaseline= "top";
		ctx.font= "20px Georgia";
		ctx.fillStyle= "#fff";
		ctx.fillText(text,x*ratio+5,y*ratio);
	}
	//判断当前浏览器是否支持WebSocket
	if ('WebSocket' in window) {
	    websocket = new WebSocket("ws://192.168.1.129:1234");
	}
	else {
	    alert('当前浏览器 Not support websocket')
	}
	//连接发生错误的回调方法
	websocket.onerror = function () {
	    alert("与检测服务器断开连接，请刷新页面重新建立连接");
	};
	//连接成功建立的回调方法
	websocket.onopen = function () {
	    setMessageInnerHTML("loading");
	}
	//接收到消息的回调方法
	websocket.onmessage = function (event) {
	    // setMessageInnerHTML(event.data);
		// 相机编号
		if(poll_num!=event.data.slice(0,1)) {
			poll_num= event.data.slice(0,1);
			$(".tab_camera .btn").removeClass("choosed");
			$(".tab_camera .camera"+poll_num+"_btn").addClass("choosed");
			console.log(arr_url[0]);
			$(".cameracase input").val(arr_url[poll_num-1]);
			gotoPlay();
		}
		// 框相关数据x,y,w,h
		let s= event.data.split(";");
		// let x= Math.abs(s[0]-s[2]/2);
		// let y= Math.abs(s[1]-s[3]/2);
		ctx.clearRect(0,0,d.width,d.height);
		// '+'make string to number
		// rect(x,y,+s[2],+s[3]);
		for(let i=0;i<s.length-1;i++) {
			let arr= s[i].split(",");
			let info= arr[0].slice(2);
			let x= Math.abs(arr[1]-arr[3]/2);
			let y= Math.abs(arr[2]-arr[4]/2);
			let w= +arr[3];
			let h= +arr[4];
			rect(x,y,w,h);
			text(info,x,y);
			// console.log(info)
			// for(let j=0;j<arr.length;j++) {
			// }
		}

	}
	//连接关闭的回调方法
	websocket.onclose = function () {
	    // alert("与检测服务器断开连接，请刷新页面重新建立连接");
	}
	//监听窗口关闭事件，当窗口关闭时，主动去关闭websocket连接，防止连接还没断开就关闭窗口，server端会抛异常。
	window.onbeforeunload = function () {
	    closeWebSocket();
	}
	//将消息显示在网页上
	function setMessageInnerHTML(data) {
	    console.log(data);
	}
	//关闭WebSocket连接
	function closeWebSocket() {
	    websocket.close();
	}
})