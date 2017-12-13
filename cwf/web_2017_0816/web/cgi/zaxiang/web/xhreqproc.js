/************************************************************
*file:queryjs
*brief:web common js module
*author:pengkunfan
*time:2014-10-20
************************************************************/


/************************common module!*************************************/

/*
* open version txt
*/
function openwin()
{
		window.open("./version.txt","关于","width=200,heigth=200");
}

/*
 *创建异步访问对象
 */
function createXHR() 
{
    var xhr;

    try 
    {
        xhr = new ActiveXObject("Msxml2.XMLHTTP");
    } 
    catch (e) 
    {
        try 
        {// code for IE6, IE5
            xhr = new ActiveXObject("Microsoft.XMLHTTP");
        }
        catch(E) 
        {
            xhr = false;
        }
    }

    if (!xhr && typeof XMLHttpRequest != 'undefined') 
    {//code for IE7+, Firefox, Chrome, Opera, Safari
        xhr = new XMLHttpRequest();
    }

    return xhr;
}


/*******************************datetime start !**************************************/


/*
 *异步访问提交处理
 */
function sender() 
{
    xhr = createXHR();

    if(xhr)
    {
        xhr.onreadystatechange=callbackFunction;
   
        xhr.open("POST", "cgi-bin/test.cgi?cur_time=" + new Date().getTime(),true);
		//xhr.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
        xhr.send("HAI");

    }
    else
    {
        //XMLHttpRequest对象创建失败
        alert("浏览器不支持，请更换浏览器！");
    }
}

/*
 *异步回调函数处理
 */
function callbackFunction()
{
	var returnValue = "  ";
    if (xhr.readyState == 4 ||xhr.readyState=="complete") 
    {
        if (xhr.status == 200) 
        {
            returnValue = xhr.responseText;

            if(returnValue != null && returnValue.length > 0)
            {
                document.getElementById("current_time").innerHTML = returnValue;
            }
            else
            {
                alert("结果为空！");
            }
        } 
        else 
        {
           // alert("页面出现异常！");
        }
    }
}

/*
* set timer
*/
function time_loop_refresh()
{
	//setTimeout("sender()",1000);
	setInterval("sender()",1000);
	
}

/*****************************index start !*************************************/

/*
* confirm reboot for index video reboot  
*/
function systemReboot()
{
	//if (confirm("确定要重启系统吗？") == 1)
	//{
		//alert("与设备的连接将断开，请稍后重新登录");
		//document.getElementById("player").SystemReboot();
	//}
	//else 
	//{
	//}
return true;
	//if(confirm("你确定要重启系统?")==1)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
}

function enc_init()
{	
		
		//var tmp_select_id1 = document.getElementById("select_batch").value;
		//var tmp_select_id2 = document.getElementById("rtsp_addr").value = obj.value;
		var xmlhttp =createXHR();
		
	//	alert("111");
		var obj_fps = document.getElementById("select_fps_id");
		var obj_res = document.getElementById("select_resolution_id");
		var obj_bit_ctrl = document.getElementById("select_bit_ctrl_id");
		
		//alert("222");
		
		//var title = document.getElementById('testid');
		//for(var i=0;i<obj.options.length;i++){
		//if(obj.options[i].innerHTML == 'test'){
		//obj.options[i].selected = true;
		//break;
	//	}
		//}
		
		xmlhttp.onreadystatechange=function()
		  {
	
		  if (xmlhttp.readyState==4 && xmlhttp.status==200)
			{ 
				var i=0;
				var j=0;
				var bit_rate="";
				var resolution="";
				var fps="";
				var bit_ctrl="";
					
				for(i=0;i<xmlhttp.responseText.length;i++)
				{
					if(xmlhttp.responseText[i]!='\n'&&j==0)
					{
						bit_rate+=xmlhttp.responseText[i];
					}
					else if(xmlhttp.responseText[i]!='\n'&&j==1)
					{
						
						resolution += xmlhttp.responseText[i];
					}
					else if(xmlhttp.responseText[i]!='\n'&&j==2)
					{
						
						fps += xmlhttp.responseText[i];
					}
					else if(xmlhttp.responseText[i]!='\n'&&j==3)
					{
						
						bit_ctrl += xmlhttp.responseText[i];
					}
					else j++
				}

				document.getElementById("bir_rate_id0").value = bit_rate;
				//alert(resolution);
				//alert(fps);
				if(resolution == "1080p")
				{
					obj_res.options[0].selected = true;
				}
				else if(resolution == "720p")
				{
					obj_res.options[1].selected = true;
				}
				else if(resolution == "1080i")
				{
					obj_res.options[2].selected = true;
				}

				else if(resolution == "480p")
				{
					obj_res.options[3].selected = true;
				}

				if("60fps" == fps)
				{
					obj_fps.options[0].selected = true;
				}
				else if("30fps" == fps)
				{
					obj_fps.options[1].selected = true;
				}
				else if("25fps" == fps)
				{
					obj_fps.options[2].selected = true;
				}

				if("CBR" == bit_ctrl)
				{
					obj_bit_ctrl.options[0].selected = true;
				}
				else if("VBR" == bit_ctrl)
				{
					obj_bit_ctrl.options[1].selected = true;
				}

	
			}
			
		  }
		  
		var url = "cgi-bin/boot_args.cgi?timestamp="+ new Date().getTime();
		var queryString = createQueryString3();
		
		xmlhttp.open("POST",url);
		//xmlhttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
		xmlhttp.send(queryString);
		//alert(document.getElementById("select_batch").options(1).selected);
}

function version_init_bk()
{
	document.getElementById("kernel_v_id").innerHTML = "你真好";
}
function version_init()
	{	
			var xmlhttp =createXHR();
			/*var obj_kernel = document.getElementById("kernel_v_id");
			var obj_fs = document.getElementById("fs_v_id");
			var obj_hw = document.getElementById("hw_v_id");
			*/
			xmlhttp.onreadystatechange=function()
			  {
		
			  if (xmlhttp.readyState==4 && xmlhttp.status==200)
				{ 
					var i=0;
					var j=0;
					var kernel_v="";
					var fs_v="";
					var hw_v="";
					var mac_v="";
					var dev_v="";
					
					for(i=0;i<xmlhttp.responseText.length;i++)
					{
						if(xmlhttp.responseText[i]!='\n'&&j==0)
						{
							kernel_v+=xmlhttp.responseText[i];
						}
						else if(xmlhttp.responseText[i]!='\n'&&j==1)
						{
							
							fs_v += xmlhttp.responseText[i];
						}
						else if(xmlhttp.responseText[i]!='\n'&&j==2)
						{
							
							hw_v += xmlhttp.responseText[i];
						}
						else if(xmlhttp.responseText[i]!='\n'&&j==3)
						{
							
							mac_v += xmlhttp.responseText[i];
						}
						else if(xmlhttp.responseText[i]!='\n'&&j==4)
						{
							
							dev_v += xmlhttp.responseText[i];
						}
						else j++
					}
	
					document.getElementById("kernel_v_id").innerHTML = kernel_v;
					document.getElementById("fs_v_id").innerHTML = fs_v;
					document.getElementById("hw_v_id").innerHTML = hw_v;
					document.getElementById("mac_v_id").innerHTML = mac_v;
					document.getElementById("dev_v_id").innerHTML = dev_v;
					//alert(resolution);
					//alert(fps);
					/*
					if(resolution == "1080p")
					{
						obj_res.options[0].selected = true;
					}
					else if(resolution == "720p")
					{
						obj_res.options[1].selected = true;
					}
	
					if("60fps" == fps)
					{
						obj_fps.options[0].selected = true;
					}
					else if("30fps" == fps)
					{
						obj_fps.options[1].selected = true;
					}
					else if("25fps" == fps)
					{
						obj_fps.options[2].selected = true;
					}
	
					*/
				}
				
			  }
			  
			var url = "cgi-bin/boot_args.cgi?timestamp="+ new Date().getTime();
			var queryString = createQueryString5();
			
			xmlhttp.open("POST",url);
			//xmlhttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
			xmlhttp.send(queryString);
			//alert(document.getElementById("select_batch").options(1).selected);
	}



function client_init()
{	
		
		//var tmp_select_id1 = document.getElementById("select_batch").value;
		//var tmp_select_id2 = document.getElementById("rtsp_addr").value = obj.value;
		var xmlhttp =createXHR();
		
	//	alert("111");
		var obj_client_type = document.getElementById("client_type_id");

		
		
		//alert("222");
		
		//var title = document.getElementById('testid');
		//for(var i=0;i<obj.options.length;i++){
		//if(obj.options[i].innerHTML == 'test'){
		//obj.options[i].selected = true;
		//break;
	//	}
		//}
		
		xmlhttp.onreadystatechange=function()
		  {
	
		  if (xmlhttp.readyState==4 && xmlhttp.status==200)
			{ 
				var i=0;
				var j=0;
				var client_type="";
				
				for(i=0;i<xmlhttp.responseText.length;i++)
				{
					if(xmlhttp.responseText[i]!='\n'&&j==0)
					{
						
						client_type += xmlhttp.responseText[i];
					}
					else j++
				}

			
				//alert(client_type);
				//alert(fps);

				if("pc" == client_type)
				{
					obj_client_type.options[0].selected = true;
				}
				else if("decode" == client_type)
				{
					obj_client_type.options[1].selected = true;
				}
				else if("android" == client_type)
				{
					obj_client_type.options[2].selected = true;
				}

	
			}
			
		  }
		  
		var url = "cgi-bin/boot_args.cgi?timestamp="+ new Date().getTime();
		var queryString = createQueryString4();
		
		xmlhttp.open("POST",url);
		//xmlhttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
		xmlhttp.send(queryString);
		//alert(document.getElementById("select_batch").options(1).selected);
}
function validateParam()
{
	
	
	var bit_rate = document.getElementById("bir_rate_id0").value;
	var temp = parseInt(bit_rate);
	if(temp < 512000 || temp > 12000000)
	{
		alert("码率应大于512K，小于12M，请检查设置！！");
		return false;
	}
	return true;
}

/*
*	check systemparam 

*/
function systemParam()
{
	//var mybatch = document.getElementById("select_batch").value;

	/****qjq add start***/
	var mychannel = document.getElementById("mychannel").value;
	
	/****qjq add end***/

	
	var mychannel = document.getElementById("mychannel").value;
	if(mychannel=="one")
	{

	var rtsp_address1 = document.getElementById("rtsp_addr1").value;
	if(rtsp_address1=="")
	{
		alert("通道1码流地址不能为空！");
		return false;
	}
	return true;
	}
	else 	if(mychannel=="two")
	{

	var rtsp_address1 = document.getElementById("rtsp_addr1").value;
	var rtsp_address2 = document.getElementById("rtsp_addr2").value;
	if(rtsp_address1==""||rtsp_address2=="")
	{
		alert("通道1,2码流地址不能为空！");
		return false;
	}
	return true;
	}
	else 	if(mychannel=="thid")
	{

	var rtsp_address1 = document.getElementById("rtsp_addr1").value;
	var rtsp_address2 = document.getElementById("rtsp_addr2").value;
	var rtsp_address3 = document.getElementById("rtsp_addr3").value;
	if(rtsp_address1==""||rtsp_address2==""||rtsp_address3=="")
	{
		alert("通道1,2,3码流地址不能为空！");
		return false;
	}

	return true;
	}
		else 	if(mychannel=="four")
	{

	var rtsp_address1 = document.getElementById("rtsp_addr1").value;
	var rtsp_address2 = document.getElementById("rtsp_addr2").value;
	var rtsp_address3 = document.getElementById("rtsp_addr3").value;
	var rtsp_address4 = document.getElementById("rtsp_addr4").value;
	if(rtsp_address1==""||rtsp_address2==""||rtsp_address3==""||rtsp_address3=="")
	{
		alert("通道1,2,3,4码流地址不能为空！");
		return false;
	}
	else
	return true;
	}
	else
	{	
		return false;
	}
	
}

function validate_pwd_Param()
{
	var user_name = document.getElementById("user_name_id1").value;
	var pwd_1 = document.getElementById("pwd_id1").value;
	var pwd_2 = document.getElementById("pwd_id2").value;

	if(user_name=="")
	{
		alert("用户名输入不能为空！");
		return false;
	}
	if((pwd_1=="") || (pwd_1==""))
	{
		alert("密码输入不能为空！");
		return false;
	}
	
	if(pwd_1 == pwd_2)
	{
		return true;
	}
	else
	{
		alert("两次输入密码不一致 !!!");
		return false;
	}

}

/*
* put param1
*/
function createQueryString1()
{
	var mybatch = "";
	mybatch =document.getElementById("select_batch").value;
	var mychannelId ="";
	mychannelId = document.getElementById("mychannel").value;
	//var mychannelsum = document.getElementById("mychannelsum").value;
	//var mytime = document.getElementById("mytime").value;
	var queryString = "id=1"+"&mybatch="+mybatch+"&mychannel=" + mychannelId+"&mychannelsum="+"2";//mychannelsum; the end is erro!
	//alert(mychannel);
	return encodeURI(encodeURI(queryString));//防止乱码
}
/*
*get rtspaddr
*/
function select_Channel(obj)
{
	var select_mychannel_id = document.getElementById("mychannel").value = obj.value;
	
}
/*
* select operator
*/
function select_opera(obj)
{	
	
	var select_batch_id = document.getElementById("select_batch").value;
	var select_mychannel_id = document.getElementById("mychannel").value = obj.value;
	
	document.getElementById("osdCheck").checked = 'ture';
	document.getElementById("osdTime").checked = 'ture';
	
	if(select_batch_id=="one")
	{
		document.getElementById("mychannel").value = "thid";
		
		document.getElementById("mytime1").disabled = "";
		document.getElementById("mytime2").disabled = "";
		document.getElementById("mytime3").disabled = "";
	}
	else
	{
		document.getElementById("mytime1").disabled = 'ture';
		document.getElementById("mytime2").disabled = 'ture';
		document.getElementById("mytime3").disabled = 'ture';
	}
	
	var xmlhttp = createXHR();
	
	xmlhttp.onreadystatechange=function()
	  {
		//alert(xmlhttp.readyState);
		//alert(xmlhttp.status);
		if (xmlhttp.readyState==4 && xmlhttp.status==200)
		{ 
			//document.getElementById("print").innerHTML= decodeURI(xmlhttp.responseText);//xmlhttp.responseText;
			//alert(xmlhttp.responseText);
			var myrtsp1 ="";
			var myrtsp2 ="";
			var myrtsp3 ="";
			var myrtsp4 ="";
			
			var mytime ="";
			var j=0;
			var i=0;
			var k=0;
			var m=0;
			//alert(xmlhttp.responseText.length);
			//alert(xmlhttp.responseText);
			//alert(xmlhttp.responseText.indexOf("rtsp"));
				var offset=0;
				var count=0;//"rtsp";
			//for(i=0;i<xmlhttp.responseText.length;i++)
			var subStr ="rtsp";
			var arrayTime = new Array([4]);
			var arrayRtsp = new Array(4);　
			arrayTime[0]="";　
			arrayTime[1]="";　
			arrayTime[2]="";　
			arrayTime[3]="";
			
			arrayRtsp[0]="";　
			arrayRtsp[1]="";　
			arrayRtsp[2]="";　
			arrayRtsp[3]="";
			var mychannelId ="";
			mychannelId = document.getElementById("mychannel").value;
			if(mychannelId =="one")k=1;
			else if(mychannelId =="two")k=2;
			else if(mychannelId =="thid")k=3;
			else if(mychannelId =="four")k=4;
			
	
			for(i=0;i<xmlhttp.responseText.length;i++)
			{
				//alert(xmlhttp.responseText[i]=='');
				//alert(j);
				if(xmlhttp.responseText[i]!='\n'&&j!=k)
				{
					
					
					arrayRtsp[j]+=xmlhttp.responseText[i];
					//alert(arrayRtsp[j]);
				}
				else if(xmlhttp.responseText[i]!='\n'&&j>=k&&xmlhttp.responseText[i]!=';')
				{
					//mytime+=xmlhttp.responseText[i];
					arrayTime[m]+=xmlhttp.responseText[i];
					//alert("time");
					//alert(mytime);
				}
				else if(xmlhttp.responseText[i]=='\n')
				{
					j++;
				}
				else if(xmlhttp.responseText[i]==';')
				{
					m++;
				}
				
				//alert(xmlhttp.responseText[i]);
			}
			for(i=0; i<j; i++)
			{
				
				//arrayRtsp[i]+=xmlhttp.responseText[j];
				//alert(arrayRtsp[i]);
			}
			//alert(m);
			/*
			for(i=0;i<xmlhttp.responseText.length;i++)
			{
				//alert(xmlhttp.responseText[i]=='');
				if(xmlhttp.responseText[i]!='\n'&&j==0)
				{
					myrtsp1+=xmlhttp.responseText[i];
				}
				else if(xmlhttp.responseText[i]!='\n'&&j==1)
				{
					mytime+=xmlhttp.responseText[i];
				}
				else j++;
				//alert(xmlhttp.responseText[i]);
			}*/
			
			//if(myrtsp1=="")myrtsp1="rtsp://192.168.0.102/test.264";
			//if(mytime=="")mytime=10;
			if(j==1)
			{
				document.getElementById("rtsp_addr1").value=arrayRtsp[0];
			}
			else if(j==2)
			{	
				document.getElementById("rtsp_addr1").value = arrayRtsp[0];
				document.getElementById("rtsp_addr2").value = arrayRtsp[1];
			}
			else if(j==3)
			{
				document.getElementById("rtsp_addr1").value = arrayRtsp[0];
				document.getElementById("rtsp_addr2").value = arrayRtsp[1];
				document.getElementById("rtsp_addr3").value = arrayRtsp[2];
			}
			else if(j==4)
			{
				document.getElementById("rtsp_addr1").value = arrayRtsp[0];
				document.getElementById("rtsp_addr2").value = arrayRtsp[1];
				document.getElementById("rtsp_addr3").value = arrayRtsp[2];
				document.getElementById("rtsp_addr4").value = arrayRtsp[3];
			}
			//document.getElementById("rtsp_addr1").value = myrtsp1;
			if(select_batch_id=="one")
			{
				
				document.getElementById("mytime1").value = arrayTime[0];
				document.getElementById("mytime2").value = arrayTime[1];
				document.getElementById("mytime3").value = arrayTime[2];
			}
		}
		
	  }

	var url = "cgi-bin/boot_args.cgi?timestamp="+ new Date().getTime();
	var queryString = createQueryString1();
	xmlhttp.open("POST",url);
	//xmlhttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
	//alert(queryString);
	xmlhttp.send(queryString);
	//alert(document.getElementById("select_batch").options(1).selected);
}

/*
* init html page
*/
function initPage()
{
	var select_batch_id = document.getElementById("select_batch").value;
	var select_mychannel_id = document.getElementById("mychannel").value;
	
	
	if(select_batch_id!="one")
	{
		document.getElementById("select_batch").value ="one";
	}
	if(select_mychannel_id !="thid")
	{
		document.getElementById("mychannel").value = "thid";
	}
	select_opera(this);
}

/************************** ipaddr start !***************************************/
/*
* confirm reboot for ipaddr 
*/
function configrestart()
{
if (confirm("确定要修改IP吗？") == 1)
		{
			
			var pkf;
			//alert("fdfdfkdj");
			pkf=document.getElementById('ip_addr').value;
			alert("与设备的连接将断开，请稍后重新登录"+pkf);
			//alert(pkf);
			//document.getElementById("player").SystemReboot();
			return true;
		}
		else 
		{
			return false;
		}
}
 
 /*
 * queryString put server
 */
function createQueryString2()
{

	var queryString = "id=2";
	return encodeURI(encodeURI(queryString));//防止乱码
}
 function createQueryString3()
 {
 
	 var queryString = "id=3";
	 return encodeURI(encodeURI(queryString));//防止乱码
 }
 function createQueryString4()
 {
 
	 var queryString = "id=4";
	 return encodeURI(encodeURI(queryString));//防止乱码
 }
  function createQueryString5()
 {
 
	 var queryString = "id=5";
	 return encodeURI(encodeURI(queryString));//防止乱码
 }
    function createQueryString6()
 {
 
	 var queryString = "id=6";
	 return encodeURI(encodeURI(queryString));//防止乱码
 }
	function createQueryString50()
 {
 
	 var queryString = "id=50";
	 return encodeURI(encodeURI(queryString));//防止乱码
 }
		function createQueryString51()
 {
 
	 var queryString = "id=51";
	 return encodeURI(encodeURI(queryString));//防止乱码
 }

		function createQueryString55()
 {
 
	 var queryString = "id=55";
	 return encodeURI(encodeURI(queryString));//防止乱码
 }
  function ipaddr_get_select()
  {   
	  
	  //var tmp_select_id1 = document.getElementById("select_batch").value;
	  //var tmp_select_id2 = document.getElementById("rtsp_addr").value = obj.value;
	  var xmlhttp =createXHR();
	  
	  xmlhttp.onreadystatechange=function()
		{
  
		if (xmlhttp.readyState==4 && xmlhttp.status==200)
		  { 
			  //document.getElementById("print").innerHTML= decodeURI(xmlhttp.responseText);//xmlhttp.responseText;
			  //alert(xmlhttp.responseText);
			  //alert(xmlhttp.responseText.length);
			  var i=0;
			  var j=0;
			  var ip="";
			  var mask="";
			  var gateway="";
		  
			  for(i=0;i<xmlhttp.responseText.length;i++)
			  {
				  //alert(xmlhttp.responseText[i]=='');
				  if(xmlhttp.responseText[i]!='\n'&&j==0)
				  {
					  ip+=xmlhttp.responseText[i];
				  }
				  else if(xmlhttp.responseText[i]!='\n'&&j==1)
				  {
					  
					  mask+=xmlhttp.responseText[i];
				  }
				  else if(xmlhttp.responseText[i]!='\n'&&j==2)
				  {
					  
					  gateway+=xmlhttp.responseText[i];
				  }
				  else j++
			  }
			  //alert(ip);
			  //alert(mask);
			  //alert(gateway);
			  document.getElementById("ip_addr").value = ip;
			  document.getElementById("net_mask").value = mask;
			  document.getElementById("gate_way").value = gateway;
			  //alert(xmlhttp.statusText);
			  //alert(xmlhttp.status);
  
		  }
		  
		}
		
	  var url = "cgi-bin/boot_args.cgi?timestamp="+ new Date().getTime();
	  var queryString = createQueryString2();
	  
	  xmlhttp.open("POST",url);
	  //xmlhttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
	  xmlhttp.send(queryString);
	  //alert(document.getElementById("select_batch").options(1).selected);
  }



  function facedetection_serversetting()
  {   
	  
	  //var tmp_select_id1 = document.getElementById("select_batch").value;
	  //var tmp_select_id2 = document.getElementById("rtsp_addr").value = obj.value;
	  var xmlhttp =createXHR();
	  
	  xmlhttp.onreadystatechange=function()
		{
  
		if (xmlhttp.readyState==4 && xmlhttp.status==200)
		  { 
			  //document.getElementById("print").innerHTML= decodeURI(xmlhttp.responseText);//xmlhttp.responseText;
			  //alert(xmlhttp.responseText);
			  //alert(xmlhttp.responseText.length);
			  var i=0;
			  var j=0;
			  var severip="";
			  var severport="";
			
		  
			  for(i=0;i<xmlhttp.responseText.length;i++)
			  {
				  //alert(xmlhttp.responseText[i]=='');
				  if(xmlhttp.responseText[i]!='\n'&&j==0)
				  {
					  severip+=xmlhttp.responseText[i];
				  }
				  else if(xmlhttp.responseText[i]!='\n'&&j==1)
				  {
					  
					  severport+=xmlhttp.responseText[i];
				  }
				 
				  else j++
			  }
			  //alert(ip);
			  //alert(mask);
			  //alert(gateway);
			  document.getElementById("server_ipaddr").value = severip;
			  document.getElementById("server_port").value = severport;
			  
			  //alert(xmlhttp.statusText);
			  //alert(xmlhttp.status);
  
		  }
		  
		}
		
	  var url = "cgi-bin/boot_args.cgi?timestamp="+ new Date().getTime();
	  var queryString = createQueryString55();
	  
	  xmlhttp.open("POST",url);
	  //xmlhttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
	  xmlhttp.send(queryString);
	  //alert(document.getElementById("select_batch").options(1).selected);
  }




function audio_init()
{
	var xmlhttp =createXHR();
	
	xmlhttp.onreadystatechange=function()
	  {

	  if (xmlhttp.readyState==4 && xmlhttp.status==200)
		{ 
			//document.getElementById("print").innerHTML= decodeURI(xmlhttp.responseText);//xmlhttp.responseText;
			//alert(xmlhttp.responseText);
			//alert(xmlhttp.responseText.length);
			var i=0;
			var j=0;
			var pb_volume="";
			var r_volume="";

		
			for(i=0;i<xmlhttp.responseText.length;i++)
			{
				//alert(xmlhttp.responseText[i]=='');
				if(xmlhttp.responseText[i]!='\n'&&j==0)
				{
					pb_volume+=xmlhttp.responseText[i];
				}
				else if(xmlhttp.responseText[i]!='\n'&&j==1)
				{
					
					r_volume+=xmlhttp.responseText[i];
				}

				else j++
			}
			//alert(ip);
			//alert(mask);
			//alert(gateway);

	/*	var tt;
		var r = confirm(pb_volume);
		if(r==true)
		{
			tt = "Y"
		}
		else
		{
			tt="N";
		}
			
	*/
			document.getElementById("pb_volume_id0").value = pb_volume;
			document.getElementById("r_volume_id0").value = r_volume;

			//alert(xmlhttp.statusText);
			//alert(xmlhttp.status);

		}
		
	  }
	  
	var url = "cgi-bin/boot_args.cgi?timestamp="+ new Date().getTime();
	var queryString = createQueryString6();
	
	xmlhttp.open("POST",url);
	//xmlhttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
	xmlhttp.send(queryString);
	//alert(document.getElementById("select_batch").options(1).selected);
}
 
  function display_init()
{
	var xmlhttp =createXHR();
	var obj_start_opencv= document.getElementById("startopencv");
	xmlhttp.onreadystatechange=function()
	  {

	  if (xmlhttp.readyState==4 && xmlhttp.status==200)
		{ 
			var i=0;
			var j=0;
			var rtsp_url="";
			var start_opencv="";

		
			for(i=0;i<xmlhttp.responseText.length;i++)
			{
				//alert(xmlhttp.responseText[i]=='');
				if(xmlhttp.responseText[i]!='\n'&&j==0)
				{
					rtsp_url+=xmlhttp.responseText[i];
				}
				else if(xmlhttp.responseText[i]!='\n'&&j==1)
				{
					
					start_opencv+=xmlhttp.responseText[i];
				}
				

				else j++
			}
		
		
			if(start_opencv== "是")
				{
					obj_start_opencv.options[0].selected = true;
				}
				else if(start_opencv== "否")
				{
					obj_start_opencv.options[1].selected = true;
				}

		
			document.getElementById("url").value = rtsp_url;
			

		

		}
		
	  }
	  
	var url = "cgi-bin/boot_args.cgi?timestamp="+ new Date().getTime();
	var queryString = createQueryString6();
	
	xmlhttp.open("POST",url);
	//xmlhttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
	xmlhttp.send(queryString);
	//alert(document.getElementById("select_batch").options(1).selected);
}
 
 function osd_init()
 {
	 var xmlhttp =createXHR();

	var obj_osd_color = document.getElementById("camera_osdsetting_color");
	var obj_osd_time = document.getElementById("camera_osdsetting_time");

	
	xmlhttp.onreadystatechange=function()
	  {

	  if (xmlhttp.readyState==4 && xmlhttp.status==200)
		{ 
			//document.getElementById("print").innerHTML= decodeURI(xmlhttp.responseText);//xmlhttp.responseText;
			//alert(xmlhttp.responseText);
			//alert(xmlhttp.responseText.length);
			var i=0;
			var j=0;
		
			var str="";
			var size="";
			var color="";
			var x="";
			var y="";
			var device_addr ="";
			var time ="";
			
			
		
			for(i=0;i<xmlhttp.responseText.length;i++)
			{
				//alert(xmlhttp.responseText[i]=='');
				if(xmlhttp.responseText[i]!='\n'&&j==0)
				{
					str+=xmlhttp.responseText[i];
					
				}
				else if(xmlhttp.responseText[i]!='\n'&&j==1)
				{
					
					size+=xmlhttp.responseText[i];
				}
				else if(xmlhttp.responseText[i]!='\n'&&j==2)
				{
					
					color+=xmlhttp.responseText[i];
				}
				else if(xmlhttp.responseText[i]!='\n'&&j==3)
				{
					
					x+=xmlhttp.responseText[i];
				}
				else if(xmlhttp.responseText[i]!='\n'&&j==4)
				{
					
					y+=xmlhttp.responseText[i];
				}
				else if(xmlhttp.responseText[i]!='\n'&&j==5)
				{
					
					device_addr+=xmlhttp.responseText[i];
				}
				else if(xmlhttp.responseText[i]!='\n'&&j==6)
				{
					
					time+=xmlhttp.responseText[i];
				}
				
				

				else j++
			}



			if(color == "红色")
				{
					obj_osd_color.options[0].selected = true;
				}
				else if(color == "绿色")
				{
					obj_osd_color.options[1].selected = true;
				}
				else if(color == "蓝色")
				{
					obj_osd_color.options[2].selected = true;
				}
				else if(color == "黄色")
				{
					obj_osd_color.options[3].selected = true;
				}
				else if(color == "黑色")
				{
					obj_osd_color.options[4].selected = true;
				}
				else if(color == "白色")
				{
					obj_osd_color.options[5].selected = true;
				}

				
				if(time == "显示")
				{
					obj_osd_time.options[0].selected = true;
				}
				else if(time == "隐藏")
				{
					obj_osd_time.options[1].selected = true;
				}

		var tt;
		var r = confirm(str);
		if(r==true)
		{
			tt = "Y"
		}
		else
		{
			tt="N";
		}
		
	
			
			
			document.getElementById("camera_osdsetting_str").value =str;
			document.getElementById("camera_osdsetting_size").value = size;
			//document.getElementById("camera_osdsetting_color").value = color;
			document.getElementById("camera_osdsetting_location1").value = x;
			document.getElementById("camera_osdsetting_location2").value = y;
			document.getElementById("camera_osdsetting_device_addr").value = device_addr;
		
			//document.getElementById("camera_osdsetting_time").value = time;
			
		

			
			

		}
		
	  }
	  
	var url = "cgi-bin/boot_args.cgi?timestamp="+ new Date().getTime();
	var queryString = createQueryString50();
	
	xmlhttp.open("POST",url);
	//xmlhttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
	xmlhttp.send(queryString);
	//alert(document.getElementById("select_batch").options(1).selected);

 }

 function arithmeticpara_init()
 {
	var xmlhttp =createXHR();

	var obj_arithmetic_recvideo = document.getElementById("arithmetic_recvideo");

	
	xmlhttp.onreadystatechange=function()
	  {

	  if (xmlhttp.readyState==4 && xmlhttp.status==200)
		{ 
			//document.getElementById("print").innerHTML= decodeURI(xmlhttp.responseText);//xmlhttp.responseText;
			//alert(xmlhttp.responseText);
			//alert(xmlhttp.responseText.length);
			var i=0;
			var j=0;
			var arithmeticpara1="";
			var arithmeticpara2="";
			var arithmeticpara3="";
			var arithmetic_recvideo ="";
		
			for(i=0;i<xmlhttp.responseText.length;i++)
			{
				//alert(xmlhttp.responseText[i]=='');
				if(xmlhttp.responseText[i]!='\n'&&j==0)
				{
					arithmeticpara1+=xmlhttp.responseText[i];
				}
				else if(xmlhttp.responseText[i]!='\n'&&j==1)
				{
					
					arithmeticpara2+=xmlhttp.responseText[i];
				}
				else if(xmlhttp.responseText[i]!='\n'&&j==2)
				{
					
					arithmeticpara3+=xmlhttp.responseText[i];
				}
				else if(xmlhttp.responseText[i]!='\n'&&j==3)
				{
					
					arithmetic_recvideo+=xmlhttp.responseText[i];
				}

				else j++
			}
			//alert(ip);
			//alert(mask);
			//alert(gateway);





				if(obj_arithmetic_recvideo == "是")
				{
					obj_arithmetic_recvideo.options[0].selected = true;
				}
				else if(obj_arithmetic_recvideo == "否")
				{
					obj_arithmetic_recvideo.options[1].selected = true;
				}


			
			document.getElementById("arithmeticpara_id1").value = arithmeticpara1;
			document.getElementById("arithmeticpara_id2").value = arithmeticpara2;
			document.getElementById("arithmeticpara_id3").value = arithmeticpara3;
			//document.getElementById("arithmetic_recvideo").value = arithmetic_recvideo;
			//alert(xmlhttp.statusText);
			//alert(xmlhttp.status);

		}
		
	  }
	  
	var url = "cgi-bin/boot_args.cgi?timestamp="+ new Date().getTime();
	var queryString = createQueryString51();
	
	xmlhttp.open("POST",url);
	//xmlhttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
	xmlhttp.send(queryString);
	//alert(document.getElementById("select_batch").options(1).selected);
}
 
function displayOsd()
{
		var x;
		var r = confirm("确定要修改/添加OSD吗？");
		if(r==true)
		{
			x = "你按下了\"确定\"按钮!";
		}
		else
		{
			x="你按下了\"取消\"按钮!";
		}
		document.getElementById("demo").innerHTML=x;
	
}


