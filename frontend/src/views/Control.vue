<template>
    <v-container class="d-flex justify-center align-center" style="height: 100vh;">
      <v-card class="pa-6" width="400">
        <v-card-title class="text-center">Combination</v-card-title>
        <v-card-subtitle class="text-center">Enter your four-digit passcode</v-card-subtitle>
        <v-card-text>
          <v-otp-input v-model="passcode" length="4"></v-otp-input>
        </v-card-text>
        <v-card-actions class="justify-center">
          <v-btn color="primary" @click="handlePasscode">SUBMIT</v-btn>
        </v-card-actions>
        <v-alert v-if="message" type="info" class="mt-3">{{ message }}</v-alert>
      </v-card>
    </v-container>
  </template>
  
  <script>
    i// IMPORTS
import { ref,reactive,watch ,onMounted,onBeforeUnmount,computed } from "vue";  
import { useRoute ,useRouter } from "vue-router";

import { useAppStore } from "@/store/appStore"; // Import App Store
import { useMqttStore } from "@/store/mqttStore"; // Import Mqtt Store
import { storeToRefs } from "pinia";
 
 
// VARIABLES
const router      = useRouter();
const route       = useRoute();  

const passcode = ref([""]);
const Mqtt = useMqttStore(); // Use Mqtt Store
const AppStore = useAppStore(); // Use App Store


// FUNCTIONS
onMounted(()=>{
    // THIS FUNCTION IS CALLED AFTER THIS COMPONENT HAS BEEN MOUNTED
    Mqtt.connect(); // Connect to Broker located on the backend
    setTimeout(() => {
      // Subscribe to each topic
      Mqtt.subscribe("620155671");
      Mqtt.subscribe("620155671_sub");
    }, 3000);
});


onBeforeUnmount(()=>{
    // THIS FUNCTION IS CALLED RIGHT BEFORE THIS COMPONENT IS UNMOUNTED
    Mqtt.unsubcribeAll();
});

function get_passcode() {
    AppStore.set_password(passcode.value);
    
}
  </script>
  
  <style scoped>
  .v-card {
    text-align: center;
  }
  </style>
  
