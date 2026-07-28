// ================================
// MQTT CONFIGURATION
// ================================


const host =
"wss://9b902896ff004582a696e5037b584b4f.s1.eu.hivemq.cloud:8884/mqtt";



const options = {

username:
"yogailmi19",

password:
"Yogayoga1919!@",

clean:true,

connectTimeout:4000

};




// Connect MQTT


const client =
mqtt.connect(
host,
options
);




// Topic

const topic =
"rumah/energi/data";




// ================================
// MQTT CONNECT
// ================================


client.on(
"connect",
()=>{


console.log(
"MQTT Connected"
);


client.subscribe(
topic
);


}

);





// ================================
// CHART CONFIG
// ================================


let waktu=[];

let dayaData=[];



const chart =
new Chart(

document.getElementById(
"grafikDaya"
),

{


type:"line",


data:{


labels:waktu,


datasets:[{


label:
"Daya (Watt)",


data:
dayaData,


tension:0.3


}]


},


options:{


responsive:true,


scales:{


y:{


beginAtZero:true


}


}


}


}

);




// ================================
// RECEIVE MQTT DATA
// ================================


client.on(
"message",
(topic,message)=>{


let data =
JSON.parse(
message.toString()
);



console.log(data);



// tampilkan data


document.getElementById(
"status"
).innerHTML =
data.status;



document.getElementById(
"tegangan"
).innerHTML =
data.tegangan.toFixed(2);



document.getElementById(
"arus"
).innerHTML =
data.arus.toFixed(2);



document.getElementById(
"daya"
).innerHTML =
data.daya.toFixed(2);



document.getElementById(
"energi"
).innerHTML =
data.energi.toFixed(3);



document.getElementById(
"frekuensi"
).innerHTML =
data.frekuensi.toFixed(2);



document.getElementById(
"pf"
).innerHTML =
data.pf.toFixed(2);




// ================================
// STATUS COLOR
// ================================


let status =
document.getElementById(
"status"
);



if(data.status=="Beban Aktif"){


status.style.color="green";


}


else if(
data.status=="Tidak Ada Beban"
){


status.style.color="gray";


}


else{


status.style.color="red";


}





// ================================
// UPDATE GRAPH
// ================================


let jam =
new Date()
.toLocaleTimeString();



waktu.push(jam);


dayaData.push(
data.daya
);



if(waktu.length>20){


waktu.shift();

dayaData.shift();


}



chart.update();



}

);
