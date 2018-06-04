/****************************************************************************** 
NVE current sensor Arduino/Involt demo, rev. 12/1/17
******************************************************************************/

setInterval(function(){
    $(document).ready(function(){

//Display RMS current:
    var gauge = document.getElementsByTagName('canvas')[0];
    gauge.setAttribute('data-value', involtReceivedPin[14]/8.65); }); },10);