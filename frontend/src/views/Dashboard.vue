<template>
    <v-container>
      <v-row>
        <v-col>
          <v-sheet>
            <v-slider v-model="radar"
            label="height (in)"
            color="green"
            class="my-slider"
            vertical/>
          </v-sheet>
        </v-col>
        <v-col>
          <v-sheet>
            <highcharts :options="chartOptions" />
          </v-sheet>
        </v-col>
      </v-row>
    </v-container>
  </template>
  
  <script setup>
  // STATES
    // const mqtt = ref(null);
    // const host = ref("http://localhost.com"); // Host Name or IP address
    // const port = ref(9002); // Port number

    // import { useMqttStore } from '@/store/mqttStore'; // Import Mqtt Store
    // import { storeToRefs } from "pinia";
    // // VARIABLES
    // const Mqtt = useMqttStore();
    // const { payload, payloadTopic } = storeToRefs(Mqtt);

    // onMounted(()=>{
    // // THIS FUNCTION IS CALLED AFTER THIS COMPONENT HAS BEEN MOUNTED
    // Mqtt.connect(); // Connect to Broker located on the backend
    // setTimeout( ()=>{
    // // Subscribe to each topic
    //     Mqtt.subscribe("topic1");
    //     Mqtt.subscribe("topic2");
    // },3000);
    // });

    // onBeforeUnmount(()=>{
    // // THIS FUNCTION IS CALLED RIGHT BEFORE THIS COMPONENT IS UNMOUNTED
    // // unsubscribe from all topics
    //     Mqtt.unsubcribeAll();
    // });

  import { ref } from 'vue';
  import Highcharts from 'highcharts';
//   import HighchartsVue from 'highcharts-vue';
  
  const radar = ref(0);
  const chartOptions = ref({
    chart: {
      type: 'area',
    },
    title: {
      text: 'Reserve Data',
    },
    xAxis: {
      type: 'datetime',
      tickPixelInterval: 150,
    },
    yAxis: {
      title: {
        text: 'Value',
      },
    },
    series: [
      {
        name: 'Reserve',
        data: [],
      },
    ],
  });
  
  // Function to update chart data with live data
  function updateChartData(newData) {
    const now = Date.now();
    chartOptions.value.series[0].data.push([now, newData]);
  
    // Keep only the last 10 minutes of data
    const tenMinutesAgo = now - 10 * 60 * 1000;
    chartOptions.value.series[0].data = chartOptions.value.series[0].data.filter(
      (point) => point[0] >= tenMinutesAgo
    );
  }
  
  // Simulate live data update every minute
  setInterval(() => {
    const newData = Math.random() * 100; // Replace with actual data fetching logic
    updateChartData(newData);
  }, 60000);
  </script>
  
  <style scoped>
  /* Add any custom styles here */
  .my-slider {
  width: 100%;
 }
  </style>