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
    import { useMqttStore } from '@/store/mqttStore'; // Import Mqtt Store
    import { storeToRefs } from "pinia";
    // VARIABLES
    const Mqtt = useMqttStore();
    const { payload, payloadTopic } = storeToRefs(Mqtt);

    // onMounted(()=>{
    //     // THIS FUNCTION IS CALLED AFTER THIS COMPONENT HAS BEEN MOUNTED
    //     Mqtt.connect(); // Connect to Broker located on the backend
    //     setTimeout( ()=>{
    //     // Subscribe to each topic
    //     Mqtt.subscribe("620155671");
    //     Mqtt.subscribe("620155671_sub");
    //     },3000);
    // });

//     onBeforeUnmount(()=>{
// // THIS FUNCTION IS CALLED RIGHT BEFORE THIS COMPONENT IS UNMOUNTED
// // unsubscribe from all topics
//         Mqtt.unsubcribeAll();
//     });

  export default {
    data() {
      return {
        passcode: '',
        message: ''
      };
    },
    methods: {
      async checkPasscode() {
        try {
          const response = await fetch('/api/check_passcode', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ passcode: this.passcode })
          });
  
          const data = await response.json();
          return data.count; // Expecting a count from check_passcode
        } catch (error) {
          console.error('Error checking passcode:', error);
          return 0;
        }
      },
  
      async updatePasscode() {
        try {
          const response = await fetch('/api/update_passcode', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ passcode: this.passcode })
          });
  
          const data = await response.json();
          this.message = 'Passcode updated successfully!';
          console.log('Updated passcode:', data);
        } catch (error) {
          console.error('Error updating passcode:', error);
          this.message = 'Failed to update passcode.';
        }
      },
  
      async handlePasscode() {
        if (!this.passcode || this.passcode.length !== 4) {
          this.message = 'Enter a valid 4-digit passcode!';
          return;
        }
  
        // const count = await this.checkPasscode();
        // if (count > 0) {
        //   this.updatePasscode();
        // } else {
        //   this.message = 'Passcode does not exist!';
        // }
      }
    }
  };
  </script>
  
  <style scoped>
  .v-card {
    text-align: center;
  }
  </style>
  