const host =
"wss://9b902896ff004582a696e5037b584b4f.s1.eu.hivemq.cloud:8884/mqtt";

const options = {

username:
"yogailmi19",

password:
"Yogayoga1919!@",

clean:true

};



const client =
mqtt.connect(
host,
options
);



client.on(
"connect",
function(){

console.log(
"MQTT Connected"
);


client.subscribe(
"rumah/energi/data"
);


}

);



let waktu=[];

let nilaiDaya=[];



const chart =
new Chart(
document.getElementById(
"grafik"
),
{

type:"line",

data:{

labels:waktu,

datasets:[{

label:"Daya Watt",

data:nilaiDaya

}]

}

}

);




client.on(
"message",
function(topic,message){


let data =
JSON.parse(
message.toString()
);



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



let sekarang =
new Date()
.toLocaleTimeString();



waktu.push(sekarang);

nilaiDaya.push(
data.daya
);



if(waktu.length>20){

waktu.shift();
nilaiDaya.shift();

}



chart.update();



});
