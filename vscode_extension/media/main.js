document.addEventListener("DOMContentLoaded", function() {
    console.log("JavaScript is working!");
});

let messages = [
    // { sender: 'user', text: 'Hello!' },
    { sender: 'IoTPilot', text: 'Hello, I\'m IoTpilot.                ' },
];


function renderMessages() {
    const chatWindow = document.getElementById('chat-window');
    const message = messages[messages.length - 1];
    const messageBox = document.createElement('div');
    messageBox.className = `message-box`;
    const icon = document.createElement('img');
    icon.src = 'placeholder_icon.png';
    const senderBox = document.createElement('div');
    senderBox.className = 'sender-box ${message.sender}';
    // senderBox.textContent = `${message.sender}:`;
    senderBox.appendChild(icon);
    senderBox.appendChild(document.createTextNode(`${message.sender}:`));
    const textBox = document.createElement('div');
    textBox.className = 'text-box';
    textBox.textContent = message.text;
    textBox.innerHTML = textBox.innerHTML.replace(/\n/g, '<br>').replace(/ /g, '&nbsp;');
    messageBox.appendChild(senderBox);
    messageBox.appendChild(textBox);
    chatWindow.appendChild(messageBox);



    chatWindow.scrollTop = chatWindow.scrollHeight;
}


async function handleSend() {
    const inputField = document.getElementById('message-input'); 
    const newMessageText = inputField.value.trim();

    if (newMessageText !== '') { 

        messages.push({sender:'user',text:newMessageText}); 


        inputField.value=''; 
        renderMessages(); 

        try {

            
            const response = await axios.post("http://47.102.103.1:20018/run ",{username:"emm" ,userproblem: newMessageText});

            const content = response['data']['result']; 
            console.log("response:\n",response);   
            console.log("content: ",content); 

            const llmMessage = {sender: 'IoTPilot', text: content}; 
            messages.push(llmMessage);

            renderMessages();
            
        }catch(error){
            console.error('Error:', error);
        }
              
   } 
}

document.getElementById('send_button').addEventListener('click', handleSend); 

document.getElementById('message-input').addEventListener('keydown', function(event){
    if (event.key == 'Enter' && !event.shiftKey){
        event.preventDefault(); // prevent default behavior
        handleSend();
    }
}); //  add event listener for keydown


// init messages
renderMessages();