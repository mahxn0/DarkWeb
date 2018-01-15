/************************************************************
*file:queryjs
*brief:web common js module
*author:chen
*time:2017/10/30
************************************************************/

/************************全局变量*************************************/
// ajax请求地址
var url= "cgi-bin/boot_args.cgi?timestamp="+ new Date().getTime();
// 浏览器类型
var browserType= window.navigator.userAgent;
// 显示的相机数量
var camera_num=1;
// 相机设置框的数量
var s=4;
// 当前播放路径
var cur_vlc_url="";
// 相机路径数组
var arr_url="";


// 提示动画
function _fadeIn() {
	$(".wrapper").fadeIn("slow");
	$(".wrapper button").click(function(){
		$(".wrapper").fadeOut("slow");
	})
}

/************************display.html*************************************/
// 浏览器窗口改变时，相机宽高
$(window).resize(
	function() {
		let object=$(".cameracase object");

		if(camera_num>=2) {
		// 	object.css("width","100%");
		// 	object.css("height","100%");
		}
	}
);

/************************settings.html*************************************/
// 相机设置value检测
function check_setting() {
	let bool= true;
	let c=document.cookie;
	let netmask= c.slice(c.indexOf("ip=")+3,c.lastIndexOf("."));
	$(".setting").map(function(i,item) {
		let ip= $(this).find(".ip_input").val();
		let cur_netmask= ip.slice(0,ip.lastIndexOf("."));
		if(netmask!==cur_netmask) {
			$(".warnning").html("相机"+(i+1)+"的网段必须为"+netmask+".x");
			_fadeIn();
			bool= false;
		}
	});
	return bool;
}
// 相机播放
function gotoPlay(){
	vlc=$("#vlc")[0];
	vlc.playlist.clear();
	vlc.playlist.playItem(vlc.playlist.add($(".mrl").val()));
}
// 后台返回数据相机设置
function setting(data) {
	let arr=data.split("begin_camera\n");
	let arr1=arr[0].split("\n").slice(1);
	let arr2=arr[1].split("\n");
	let arr3=arr[2].split("\n");
    //写入相机设置
	for(let i=0;i<arr1.length;i++) {
		let num=i+1;
		if(arr1[i]==="on") {
			$(".camera"+num+"_switch input[value='on']").attr('checked','true');
		}
	}
	for(let i=0;i<arr2.length;i++) {
		if(arr2[i]==="on") {
			let num=i+1;
			$(".camera"+num+"_check input[value='on']").attr('checked','true');
		}
	}
	for(let i=0;i<arr3.length;i++) {
		let num=i+1;
		let userMessage;
		let ip;
		if(arr3[i].slice(-13,-10)==="sub") {
			// 码流类型
			$(".camera"+num+"_stream input[value='sub']").attr('checked','true');
			// 通道号
			$("[name=camera"+num+"_setting_ch]").val(arr3[i].slice(-17,-14));
		}else {
			$(".camera"+num+"_stream input[value='main']").attr('checked','true');
			$("[name=camera"+num+"_setting_ch]").val(arr3[i].slice(-18,-15));
		}
		// 获取用户信息和ip
		arr3[i].indexOf("main")+1?userMessage=arr3[i].slice(7,-28):userMessage=arr3[i].slice(7,-27);
		$("[name=camera"+num+"_setting_username]").val(userMessage.slice(0,userMessage.indexOf(":")));
		$("[name=camera"+num+"_setting_password]").val(userMessage.slice(userMessage.indexOf(":")+1,userMessage.indexOf("@")));
		$("[name=camera"+num+"_setting_IPaddress]").val(userMessage.slice(userMessage.indexOf("@")+1));
		ip= $("[name=camera"+num+"_setting_IPaddress]");
		if(ip.val()===":") {
			ip.val("");
		}
	}
}
// 相机播放设置
function camera_setting(data) {
	let cameracase=$(".cameracase");
	let arr=data.split("begin_camera\n");
	// 相机设置框的个数
	let tab_num= arr[0].split("\n").slice(0,1);
	let tab_str= "";
	let arr1=arr[0].split("\n").slice(1);
	let arr3=arr[2].split("\n");
	// 保存相机路径数组
	arr_url= arr3;
	let on_num= arr1[0].indexOf("on")+1;

	// 相机tab选项卡
	for(let i=0;i<tab_num;i++) {
		let num= i+1;
		let str= "<div class='btn btn-default camera"+num+"_btn'>"+num+"</div>";
		tab_str+=str;
	}
	$(".camerabox .btn-group").html(tab_str);
	$(".tab_camera .camera"+on_num+"_btn").addClass("choosed");
	// 禁用未开启的相机切换
	for(let i=0;i<arr1.length;i++) {
		if(arr1[i]!=="on") {
			let num=i+1;
			$(".camera"+num+"_btn").addClass("disabled_tab").attr("title","相机未开启");
		}
	}
	$(".tab_camera").on("click",".btn",function() {
		// 获取被点击元素的索引
		let index= $(this).index();
		let i= index+1;

		if($(this).hasClass("disabled_tab")) {
			alert("相机"+i+"未开启")
		}else{
			$(".tab_camera .btn").removeClass("choosed");
			$(this).addClass("choosed");
			$(".cameracase label").html("相机"+i+" :")
			$(".cameracase input").val(arr3[index]);
			gotoPlay();
		}
	});
	// 重置相机宽度
	$(".camera object").width("");
	$(".camera object").height("");
	// 判断开启的相机
	for(let i=0;i<arr1.length;i++) {
		if(arr1[i]=='on') {
			camera_num++;
		}
	}
	// 相机初始宽高
	if(camera_num==0) {
		alert("请至少开启一个相机！")
	}
	// 初始化相机播放
	$(".cameracase label").html("相机1 :")
	$(".cameracase input").val(arr3[0]);
	gotoPlay();
}
// 动态增删相机设置栏
$(function() {
	$("form").on("click",".delete_case",function() {
		if(s<2) {
			$(".warnning").html("请至少保留一个相机设置！");
			_fadeIn();
			return false;
		}
		s--;
		$(this).parents(".setting").fadeOut("slow",function() {
			$(this).remove()
		})
		$("[name='setting_total']").val(s);
	})
	$(".add_camera").click(function() {
		if(s>7) {
			$(".warnning").html("最多只能设置八个相机！");
			_fadeIn();
			return false;
		}
		s++;
		let str="<div class='setting camera"+s+"'><p>相机"+s+"<span class='iconfont icon-shanchu delete_case'></span></p><div class='case camera"+s+"_switch'><span class='case-title'>相机开关</span><label class='radio-inline'><input type='radio' name='camera"+s+"_setting_switch' value='on'> 开</label><label class='radio-inline'><input type='radio' name='camera"+s+"_setting_switch' value='off' checked> 关</label></div><div class='case camera"+s+"_check'><span class='case-title'>检测开关</span><label class='radio-inline'><input type='radio' name='camera"+s+"_setting_check' value='on'> 开</label><label class='radio-inline'><input type='radio' name='camera"+s+"_setting_check' value='off' checked> 关</label></div><div class='case camera"+s+"_stream'><span class='case-title'>码流类型</span><label class='radio-inline'><input class='input_select setting_choose' name='camera"+s+"_setting_stream'  type='radio' value='main'> 主码流</label><label class='radio-inline'><input class='input_select setting_choose' name='camera"+s+"_setting_stream'  type='radio' value='sub' checked> 子码流</label></div><div class='case'><span class='case-title'>通道号</span><span class='triangle'>&#9660;</span>\n<input class='input_select setting_ch setting_choose' name='camera"+s+"_setting_ch' type='text' value='ch1' readonly><ul><li>ch1</li><li>ch2</li><li>ch3</li><li>ch4</li></ul></div><div class='case'><span class='case-title'>用户名</span><input name='camera"+s+"_setting_username' class='input_select inputodd' autocomplete  type='text'></div><div class='case'><span class='case-title'>密码</span><input name='camera"+s+"_setting_password' class='input_select' autocomplete type='text'></div><div class='case'><span class='case-title'> I P地址</span><input name='camera"+s+"_setting_IPaddress' class='ip_input input_select inputodd' autocomplete type='text'></div><div class='case hidden'><span class='case-title'>传输协议</span><input name='camera"+s+"_setting_protocols' class='input_select' type='text' value='rtsp' readonly></div><div class='case hidden'><span class='case-title'>端口号</span><input name='camera"+s+"_setting_port' class='input_select inputodd' type='text' value='554' readonly></div><div class='case hidden'><span class='case-title'>视频编码</span><input name='camera"+s+"_setting_coding' class='input_select' type='text' value='h264' readonly></div><div style='display: none;'><input name='none' value=''></div></div>";
		$(this).parent(".text-center").before(str);
		$("[name='setting_total']").val(s);
	})
	$("form").on("click",".input_select",function() {
		let ul = $(this).next();

		if(ul.css("display")=="none"){ 
			$(".case ul").not(ul).slideUp("slow");
			ul.slideDown("slow"); 
		}else{
			ul.slideUp("slow"); 
		}
	});
	$("form").on("click",".case ul li",function() {
		let txt = $(this).text(); 
		$(this).parent("ul").prev("input").val(txt); 
		$(this).parent("ul").slideUp("slow"); 
	})
});

/************************local.html*************************************/
// 判断文件上传类型和大小
function check_file() {
	let fileName= $("#upload").val();
	let fileType= fileName.substring(fileName.indexOf('.'));
	let filesSize;

	if(fileName=="") {
		$(".warnning").html("请先选择要上传的文件！");
		_fadeIn();
		return false;
	}else if(fileType!=".bmp"&&fileType!=".png"&&fileType!=".gif"&&fileType!=".jpg"&&fileType!=".jpeg"&&fileType!=".mp4"&&fileType!=".rmvb"&&fileType!=".avi"&&fileType!=".ts") {
		$(".warnning").html("请选择正确的文件格式！");
		_fadeIn();
		return false;
	}

	// 获取上传文件大小
	filesSize= (/msie/i.test(browserType)?$("#upload")[0].fileSize:$("#upload")[0].files[0].size);

	if(filesSize/1024/1024>=101) {
		$(".warnning").html("上传文件最大不超过100M！");
		_fadeIn();
		return false;
	}
}

/************************system.html*************************************/
// 系统设置tab选项卡
$(function() {
	let aBtn= $(".system_setting_title .btn");
	$.each(aBtn,function(i) {
		$(this).click(function() {
			aBtn.removeClass("active_tab");
			$(this).addClass("active_tab");
			$("form").not($('.form'+i)).fadeOut();
			$(".form"+i).fadeIn();
		})
	})
})

/************************change_pwd.html*************************************/
// 验证密码格式
function validate_pwd_Param() {
	let pwd_1,pwd_2;

	pwd_1= $("#pwd_id1").val();
	pwd_2= $("#pwd_id2").val();

	if($("#user_name_id1").val()=="") {
		$(".warnning").html("用户名不能为空！");
		_fadeIn();
		return false;
	}else if(pwd_1=="") {
		$(".warnning").html("密码不能为空！");
		_fadeIn();
		return false;
	}else if(pwd_1!==pwd_2) {
		$(".warnning").html("两次输入密码不一致！");
		_fadeIn();
		return false;
	}
}


/************************common module*************************************/

// 防止乱码
function encode(id){
	return encodeURI(encodeURI(id))
}


// 请求
// display.html
function display_init() {
	let id= "id=52";

	$.post(url, encode(id), function(data) {
		camera_setting(data);
	});
}

//settings.html
function settings() {
	let id= "id=52";
	$.post(url, encode(id), function(data) {
		let num=parseInt(data.slice(0,1));
		let aStr="";
		// 防止相机设置个数NAN
		if(num!==num) num=0;
		s=num;
		$("[name='setting_total']").val(s);
		for(let i=1;i<num+1;i++) {
			let str1="<div class='setting camera"+i+"'><p>相机"+i+"<span class='iconfont icon-shanchu delete_case'></span></p><div class='case camera"+i+"_switch'><span class='case-title'>相机开关</span><label class='radio-inline'><input type='radio' name='camera"+i+"_setting_switch' value='on'> 开</label><label class='radio-inline'><input type='radio' name='camera"+i+"_setting_switch' value='off' checked> 关</label></div><div class='case camera"+i+"_check'><span class='case-title'>检测开关</span><label class='radio-inline'><input type='radio' name='camera"+i+"_setting_check' value='on'> 开</label><label class='radio-inline'><input type='radio' name='camera"+i+"_setting_check' value='off' checked> 关</label></div><div class='case camera"+i+"_stream'><span class='case-title'>码流类型</span><label class='radio-inline'><input class='input_select setting_choose' name='camera"+i+"_setting_stream'  type='radio' value='main'> 主码流</label><label class='radio-inline'><input class='input_select setting_choose' name='camera"+i+"_setting_stream'  type='radio' value='sub' checked> 子码流</label></div><div class='case'><span class='case-title'>通道号</span><span class='triangle'>&#9660;</span>\n<input class='input_select setting_ch setting_choose' name='camera"+i+"_setting_ch' type='text' value='ch1' readonly><ul><li>ch1</li><li>ch2</li><li>ch3</li><li>ch4</li></ul></div><div class='case'><span class='case-title'>用户名</span><input name='camera"+i+"_setting_username' class='input_select inputodd' autocomplete  type='text'></div><div class='case'><span class='case-title'>密码</span><input name='camera"+i+"_setting_password' class='input_select' autocomplete type='text'></div><div class='case'><span class='case-title'> I P地址</span><input name='camera"+i+"_setting_IPaddress' class='ip_input input_select inputodd' autocomplete type='text'></div><div class='case hidden'><span class='case-title'>传输协议</span><input name='camera"+i+"_setting_protocols' class='input_select' type='text' value='rtsp' readonly></div><div class='case hidden'><span class='case-title'>端口号</span><input name='camera"+i+"_setting_port' class='input_select inputodd' type='text' value='554' readonly></div><div class='case hidden'><span class='case-title'>视频编码</span><input name='camera"+i+"_setting_coding' class='input_select' type='text' value='h264' readonly></div><div style='display: none;'><input name='none' value=''></div></div>";
				aStr+=str1;
		}
		$(".setting").remove();
		$("form .text-center").before(aStr);
		setting(data);
	});
}

// facedetection.html
// function facedetection_serversetting() {
// 	let id= "id=55";

// 	$.post(url, encode(id), function(data) {
// 		let arr;

// 		arr= data.split("\n");
// 		$('#server_ipaddr').val(arr[0]);
// 		$('#server_port').val(arr[1]);
// 	});
// }

// system.html
function ipaddr_get_select() {
	let id= "id=2";

	$.post( url, encode(id), function(data) {
		let arr;
		arr= data.split("\n");

		$("#ip_addr").val(arr[0]);
		$("#net_mask").val(arr[1]);
		$("#gate_way").val(arr[2]);
		$('#server_ipaddr').val(arr[3]);
		$('#server_port').val(arr[4]);
		$("#camera_polling").val(arr[5]);
	});
}

// local.html
function local_upload() {
	let id= "id=56";
	let local_video= $(".local video");
	let upLoadUrl= "cgi-bin/jc_sptp_upload.cgi?timestamp="+ new Date().getTime();
	let statu="",tested="";
	var timer1=setInterval(comet,1000);
	var timer2=setInterval(uploaded,100);

	function comet(){
		let testedUrl="",type="";
		$.post(upLoadUrl, encode(id), function(data) {
			let arr;

			arr= data.split("\n");
			// 上传图片状态
			statu= arr[0];
			// 检测进行装填
			tested= arr[1];
			// 检测文件类型
			type= arr[2];
			// 检测保存的图片路径
			testedUrl= arr[3];

			if (tested==1) {
				if(type==="image") {
					local_video.removeAttr("controls");
					local_video.attr("poster",'.'+testedUrl);
				}else {
					local_video.addAttr("controls");
					local_video.attr("src",'.'+testedUrl);
				}
				clearInterval(timer1);
			}
		});
	}
	function uploaded() {
		if (statu==1) {
			local_video.attr("poster",'./loading.gif');
			clearInterval(timer2);
		}else {
			local_video.attr("poster",'./poster.png');
		}
	}
}
// 修改ip
function configrestart() {
	if (confirm("确定要修改IP吗？") == 1) {
			alert("请切换到"+$("#ip_addr").val()+"登录");
			return true;
		}else {
			return false;
		}
}
