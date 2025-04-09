import React, { useEffect,useContext } from "react";
import { queryProviderContext } from "../Context/Context";


export default function TestCode(props) {
  const [requestState, dispatchRequest] = useContext(queryProviderContext);
  useEffect(() => {
    dispatchRequest({ type: "testError", params: { test: true }, body: {} });
  },[]);

  return (
    <div>
      <h3>Error test</h3>

    </div>
  );
}
