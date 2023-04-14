document.addEventListener('contextmenu', event => event.preventDefault());

keylogger_blocker = {
    state: false,
    attacker_info: 0,
    attacker_logs: "",
    elements:{
        active_button:document.querySelector("div.windows").querySelector("div.app.the-app").querySelector("div.active"),
        attacker_info:document.querySelector("div.attacker").querySelector("div.logger")
    },
    open: ()=>{
        console.log("<Tray Menu was triggered>");
    },
    toggle: ()=>{
        if(keylogger_blocker.attacker_info == 1){
            keylogger_blocker.attacker_info = 2;
            keylogger_blocker.attacker_logs = ""
            keylogger_blocker.elements.attacker_info.innerText = "Hey, write some text to see?";
        }
        keylogger_blocker.state = !keylogger_blocker.state;
        keylogger_blocker.elements.active_button.innerText = ["Enable","Disable"][Number(keylogger_blocker.state)]
    },
    registered_keys: [
                        "q","w","e","r","t","y","u","i","o","p",
                        "a","s","d","f","g","h","j","k","l",
                        "z","x","c","v","b","n","m",
                        "Q","W","E","R","T","Y","U","I","O","P",
                        "A","S","D","F","G","H","J","K","L",
                        "Z","X","C","V","B","N","M"
    ],
    random_key: ()=>{
        return keylogger_blocker.registered_keys[Math.round(Math.random()*(keylogger_blocker.registered_keys.length-1))];
    },
    random_keys: ()=>{
        if(!keylogger_blocker.state) return "";
        let key_len = Math.round(Math.random()*3)+4
        let the_keys = "";
        for (i=0; i<key_len; i++){
            the_keys += keylogger_blocker.random_key();
        }
        return the_keys;
    }
}

document.addEventListener("keypress", (e)=>{
    if(keylogger_blocker.attacker_info == 0){
        keylogger_blocker.attacker_info = 1;
        keylogger_blocker.attacker_logs = "Keylogger Saves:\n";
    } else if(keylogger_blocker.attacker_info == 2){
        keylogger_blocker.attacker_info = 3;
        keylogger_blocker.attacker_logs = "Keylogger Saves:\n";
    }
    let the_key = e.key;
    if(the_key == "Enter") the_key="\n";
    keylogger_blocker.attacker_logs += the_key + keylogger_blocker.random_keys() + "\0";
    keylogger_blocker.elements.attacker_info.innerText = keylogger_blocker.attacker_logs;
    keylogger_blocker.elements.attacker_info.scrollBy(0,9999);
})